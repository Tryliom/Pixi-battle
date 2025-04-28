#include "model/command/commands/create.h"

#include "manager/userManager.h"
#include "manager/iconManager.h"
#include "manager/classManager.h"

#include "model/entity/player.h"
#include "model/entity/classType/classType.h"
#include "model/entity/skill/skill.h"

#include "model/user/user.h"

void Create::onCommand(const dpp::cluster& client, const dpp::slashcommand_t& event)
{
	(new CreateInterface(event))->start();
}

CreateInterface::CreateInterface(const dpp::slashcommand_t& event) : CommandInterface(event, {
        MenuBuilder(
            [this](const std::string& value)
            {
                this->currentClass = std::stoi(value);
                if (this->page >= this->getMaxPage())
                {
                    this->page = this->getMaxPage() - 1;
                }
            },
            [this]()
            {
                std::vector<Option> options = {};
                int i = 0;
                for (ClassType* classType: this->unlockedClasses)
                {
                    Option option = Option{.label = classType->name, .description = classType->description, .value = std::to_string(i), .emoji = IconManager::getEmoji(classType->emojiName), .selected =
                    this->currentClass == i};
                    options.emplace_back(option);
                    i++;
                }

                return options;
            },
            "View a class.."
        )
}), user(UserManager::getInstance().getUser(std::to_string(event.command.usr.id)))
{
    std::string name = Utils::formatStringToMaxCharacters(event.command.usr.username, 20, false);
    this->user.player = new Player(name, ClassManager::getClass(this->user.unlockedClasses[0]));
	this->user.player->setUser(std::to_string(event.command.usr.id));
    this->rerollLeft = 100;

	for (const std::string& classId: this->user.unlockedClasses)
	{
		this->unlockedClasses.emplace_back(ClassManager::getClass(classId));
	}
}

void CreateInterface::changeScreen(CreateScreen newScreen)
{
    this->screen = newScreen;
    if (this->screen == CreateScreen::classes)
    {
        this->currentClass = 0;
    }
}

dpp::embed CreateInterface::getEmbed()
{
    std::function<void(Skill* skill, dpp::embed& embed)> addImprovements = [](Skill* skill, dpp::embed& embed)
    {
        int i = 1;
        for (SkillImprovement skillImprovement: skill->improvementsPerLevel)
        {
            if (skillImprovement.improvement == Enums::Type::Improvement::Choice)
            {
                std::string choices;
                for (SkillImprovement choice: skillImprovement.choices)
                {
                    if (!choices.empty())
                    {
                        choices += "\n";
                    }
                    choices += "- " + choice.getDescription();
                }
                embed.add_field("+" + std::to_string(i), "Choices:\n" + choices);
            }
            else
            {
                embed.add_field("+" + std::to_string(i), skillImprovement.getDescription());
            }
            i++;
        }
    };

    std::function<void(std::vector<Skill*> skills, dpp::embed& embed)> displaySkills = [&, this](std::vector<Skill*> skills, dpp::embed& embed)
    {
        int maxPage = this->getMaxPage();
        for (int i = 0; i < (this->simplifiedView ? skills.size() : maxPage); i++)
        {
            if (this->simplifiedView)
            {
                if (i < (this->page - 1) * 4 || i >= this->page * 4)
                {
                    continue;
                }
            }
            else
            {
                if (i != this->page - 1) continue;
            }
            Skill* skill = skills[i];
            std::string desc = skill->getDescription();
            std::string name = skill->formatSkillName();
            if (skill->tier != Enums::Tier::Class)
            {
                desc += "\nTier: " + Utils::toUpperCase((std::string) magic_enum::enum_name(skill->tier));
            }
            if (auto* activeSkill = dynamic_cast<ActiveSkill*>(skill))
            {
                if (activeSkill->power > 0)
                {
                    desc += "\nPower: " + std::to_string(activeSkill->power);
                }
                if (activeSkill->cooldown > 0)
                {
                    desc += "\nCooldown: " + std::to_string(activeSkill->cooldown) + " turn(s)";
                }

				desc += "\nTarget: " + Enums::targetToString[activeSkill->targetType];
			}
            embed.add_field(name, desc, true);
            if (this->simplifiedView && i % 2 == 1 && i != skills.size() - 1)
            {
                embed.add_field("\u200b", "\u200b");
            }

            if (!this->simplifiedView)
            {
                addImprovements(skill, embed);
            }
        }
    };

    if (this->screen == CreateScreen::main)
    {
        dpp::embed embed = Utils::getNeutralEmbed("Character creation");
        embed.add_field("Name", this->user.player->name);
        embed.add_field("Class", this->user.player->classType->name + (this->defaultClass ? " (default)" : ""));

        return embed;
    }
    else if (this->screen == CreateScreen::skills)
    {
        dpp::embed embed = Utils::getNeutralEmbed("Character creation - Skills");

        if (this->page > 0)
        {
            displaySkills(this->user.player->skills, embed);
        }
        else
        {
            embed.add_field("Rerolls", std::to_string(this->rerollLeft));
            embed.add_field("Evolutionary factor", StatUtility::formatStatDisplay(this->user.player->evolutionaryFactor.factor));
            embed.add_field("Next page", "Random skills");
        }

        embed.set_footer(dpp::embed_footer{.text = "Page " + std::to_string(this->page + 1) + " of " + std::to_string(this->getMaxPage())});

        return embed;
    }
    else if (this->screen == CreateScreen::classes)
    {
        dpp::embed embed = Utils::getNeutralEmbed("Character creation - Classes");
        ClassType* classType = this->unlockedClasses[this->currentClass];
        dpp::emoji* emoji = IconManager::getEmoji(classType->emojiName);
        embed.add_field((emoji != nullptr ? emoji->get_mention() + " " : "") + classType->name, classType->description);

        if (this->page > 0)
        {
            std::vector<Skill*> skills = classType->getSkills(false);
            displaySkills(skills, embed);
        }
        else
        {
            embed.add_field("Base stats", StatUtility::formatStatDisplay(classType->baseStat), true);
            embed.add_field("Stats to level 50", StatUtility::formatStatDisplay(StatUtility::addition({classType->baseStat,
                                                                                                       StatUtility::multiply(classType->statGrowth, 50)})), true);
            embed.add_field("Next page", "Skill tree");
        }

        embed.set_footer(dpp::embed_footer{.text = "Page " + std::to_string(this->page + 1) + " of " + std::to_string(this->getMaxPage())});

        return embed;
    }

    return CommandInterface::getEmbed();
}

std::vector<dpp::component> CreateInterface::getComponents()
{
    std::vector<dpp::component> components = {};

    if (this->screen == CreateScreen::main)
    {
        dpp::component row = dpp::component();
        row.add_component(dpp::component().set_label("Select skills").set_type(dpp::cot_button).set_style(dpp::cos_secondary).set_id("screen-skills"));
        row.add_component(dpp::component().set_label("Select class").set_type(dpp::cot_button).set_style(dpp::cos_secondary).set_id("screen-classes"));
        row.add_component(dpp::component().set_label("Rename").set_type(dpp::cot_button).set_style(dpp::cos_secondary).set_id("change-username"));
        row.add_component(dpp::component().set_label("Create").set_type(dpp::cot_button).set_style(dpp::cos_primary).set_id("create").set_disabled(
                this->user.player->classType == nullptr));
        components.emplace_back(row);
    }
    else if (this->screen == CreateScreen::skills)
    {
        components.emplace_back(CreateInterface::getChangePageButtons());
        dpp::component row = dpp::component();
        row.add_component(dpp::component().set_label("Back").set_type(dpp::cot_button).set_style(dpp::cos_secondary).set_id("back"));
        row.add_component(dpp::component().set_label("Reroll").set_type(dpp::cot_button).set_style(dpp::cos_secondary).set_id("reroll").set_disabled(
                this->rerollLeft == 0));
        row.add_component(dpp::component().set_label("Simplified view").set_type(dpp::cot_button).set_style(this->simplifiedView ? dpp::cos_success
                                                                                                                                 : dpp::cos_danger).set_id("simplified-view"));
        components.emplace_back(row);
    }
    else if (this->screen == CreateScreen::classes)
    {
        this->addMenuComponents(components);
        components.emplace_back(CreateInterface::getChangePageButtons());
        dpp::component row = dpp::component();
        row.add_component(dpp::component().set_label("Back").set_type(dpp::cot_button).set_style(dpp::cos_secondary).set_id("back"));
        row.add_component(dpp::component().set_label("Choose").set_type(dpp::cot_button).set_style(dpp::cos_secondary).set_id("choose"));
        row.add_component(dpp::component().set_label("Simplified view").set_type(dpp::cot_button).set_style(this->simplifiedView ? dpp::cos_success
                                                                                                                                 : dpp::cos_danger).set_id("simplified-view"));
        components.emplace_back(row);
    }

    return components;
}

void CreateInterface::onMenuClick(const dpp::select_click_t& menuClick)
{
    CommandInterface::onMenuClick(menuClick);
}

void CreateInterface::onButtonClick(const dpp::button_click_t& buttonClick)
{
    if (buttonClick.custom_id == "screen-skills")
    {
        this->changeScreen(CreateScreen::skills);
        this->page = 0;
    }
    else if (buttonClick.custom_id == "screen-classes")
    {
        this->changeScreen(CreateScreen::classes);
        this->page = 0;
    }
    else if (buttonClick.custom_id == "simplified-view")
    {
        this->simplifiedView = !this->simplifiedView;
        if (this->page >= this->getMaxPage())
        {
            this->page = this->getMaxPage() - 1;
        }
    }
    else if (buttonClick.custom_id == "back")
    {
        this->changeScreen(CreateScreen::main);
    }
    else if (buttonClick.custom_id == "reroll")
    {
        this->rerollLeft--;
        this->user.player->acquireRandomSkills();
        this->user.player->evolutionaryFactor.randomize();
        if (this->page >= this->getMaxPage())
        {
            this->page = this->getMaxPage() - 1;
        }
    }
    else if (buttonClick.custom_id == "choose")
    {
        this->user.player->switchClass(this->unlockedClasses[this->currentClass], false);
        this->changeScreen(CreateScreen::main);
        this->defaultClass = false;
    }
    else if (buttonClick.custom_id == "create")
    {
        UserManager::getInstance().saveExistingUsers();

        this->stopCollector(Utils::getGoodEmbed(
                "Character creation",
                "You have successfully created your character named **" + this->user.player->name + "** !"
        ));

		return;
    }
    else if (buttonClick.custom_id == "change-username")
    {
        dpp::interaction_modal_response modal("rename", "Rename your character");
        modal.add_component(
            dpp::component()
                .set_label("Username")
                .set_id("username")
                .set_type(dpp::cot_text)
                .set_min_length(3)
                .set_max_length(20)
                .set_text_style(dpp::text_short)
        );
        this->displayModal(modal);
    }

    CommandInterface::onButtonChangePage(buttonClick, this->getMaxPage());
    CommandInterface::onButtonClick(buttonClick);
}

void CreateInterface::onFormClick(const dpp::form_submit_t& formClick)
{
    if (formClick.custom_id == "rename")
    {
        this->user.player->name = std::get<std::string>(formClick.components[0].components[0].value);
    }

    CommandInterface::onFormClick(formClick);
}

int CreateInterface::getMaxPage() const
{
    int size = 0;

    if (this->screen == CreateScreen::skills)
    {
        size = (int) this->user.player->skills.size();
    }
    else if (this->screen == CreateScreen::classes)
    {
        size = (int) this->unlockedClasses[this->currentClass]->getSkills(false).size();
    }

    if (this->simplifiedView)
    {
        return (int) ceil((double) size / 4) + 1;
    }
    else
    {
        return (int) size + 1;
    }
}

