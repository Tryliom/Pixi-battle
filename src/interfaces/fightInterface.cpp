#include "interfaces/fightInterface.h"

#include <utility>

#include "model/entity/entity.h"
#include "model/entity/fightState.h"
#include "model/entity/classType/classType.h"
#include "model/entity/skill/skill.h"

#include "model/fight/participants.h"

#include "controller/mainController.h"
#include "manager/iconManager.h"

FightInterface::FightInterface(const dpp::slashcommand_t& event, Participants& participants) :
	CommandInterface(event, {
		MenuBuilder(
			[this](std::string value) {onChooseSkillsSelect(std::move(value));},
			[this]() {return getChooseSkillsOptions();},
			"Choose a skill"
		)
		}, true), participants(participants)
{
	menu = Menu::LOADING;
}

void FightInterface::continueGif()
{
	Gif gif;

	if (!participants.hasFightStarted)
	{
		participants.initialize();
	}

	bool playerMustPlay;
	while (!participants.isFightFinished())
	{
		participants.onFrame(gif);

		gif.addAndInitializeNewImage();

		if (participants.playerMustPlay() && participants.isAllAnimationsFinished())
		{
			if (playerMustPlay) break;
			playerMustPlay = true;
		}
		else
		{
			playerMustPlay = false;
		}
	}

	MainController::getInstance().postFight(gif.generate(), gif.generateLastFrame(), [this](const FightLink& link) {
		this->gifUrl = link.gifUrl;
		this->lastFrameUrl = "";

		if (this->participants.isFightFinished())
		{
			menu = Menu::END_FIGHT;
		}
		else
		{
			menu = Menu::START_TURN;
		}

		this->updateMessage();

		this->lastFrameUrl = link.lastFrameUrl;
	});
}

void FightInterface::start(bool assignCollector)
{
	CommandInterface::start(assignCollector);

	this->continueGif();
}

dpp::embed FightInterface::getEmbed()
{
	int leftTeamSize = participants.countByFilter(Filter{ .team = Enums::Team::Left });
	int rightTeamSize = participants.countByFilter(Filter{ .team = Enums::Team::Right });
	dpp::embed embed = Utils::getNeutralEmbed("Fight " + std::to_string(leftTeamSize) + " vs " + std::to_string(rightTeamSize));

	if (!this->lastFrameUrl.empty() || !this->gifUrl.empty())
	{
		embed.set_image(this->lastFrameUrl.empty() ? this->gifUrl : this->lastFrameUrl);
	}

	switch (menu)
	{
		case Menu::LOADING:
			embed.set_description("Loading...");
			return embed;
		case Menu::START_TURN: return getStartTurnEmbed(embed);
		case Menu::END_FIGHT: return getEndFightEmbed(embed);
		case Menu::CHOOSE_SKILLS: return getChooseSkillsEmbed(embed);
		case Menu::CHOOSE_TARGET: return getChooseTargetEmbed(embed);
        case Menu::CHOOSE_ENTITY_PART: return getChooseEntityPartEmbed(embed);
		case Menu::INSPECT_MENU: return getInspectMenuEmbed(embed);
		case Menu::INSPECT_ENTITY: return getInspectEntityEmbed(embed);
	}

	return embed;
}

std::vector<dpp::component> FightInterface::getComponents()
{
	switch (menu)
	{
		case Menu::LOADING: return {};
		case Menu::START_TURN: return getStartTurnComponents();
		case Menu::END_FIGHT: return getEndFightComponents();
		case Menu::CHOOSE_SKILLS: return getChooseSkillsComponents();
		case Menu::CHOOSE_TARGET: return getChooseTargetComponents();
        case Menu::CHOOSE_ENTITY_PART: return getChooseEntityPartComponents();
		case Menu::INSPECT_MENU: return getInspectMenuComponents();
		case Menu::INSPECT_ENTITY: return getInspectEntityComponents();
	}

	return {};
}

void FightInterface::onButtonClick(const dpp::button_click_t& buttonClick)
{
	switch (menu)
	{
		case Menu::LOADING: break;
		case Menu::START_TURN: onStartTurnButtonClick(buttonClick); break;
		case Menu::END_FIGHT: onEndFightButtonClick(buttonClick); break;
		case Menu::CHOOSE_SKILLS: onChooseSkillsButtonClick(buttonClick); break;
		case Menu::CHOOSE_TARGET: onChooseTargetButtonClick(buttonClick); break;
        case Menu::CHOOSE_ENTITY_PART: onChooseEntityPartButtonClick(buttonClick); break;
		case Menu::INSPECT_MENU: onInspectMenuButtonClick(buttonClick); break;
		case Menu::INSPECT_ENTITY: onInspectEntityButtonClick(buttonClick); break;
	}

	if (menu == Menu::FINISH_TURN)
	{
        menu = Menu::LOADING;

        CommandInterface::onButtonClick(buttonClick);

		continueGif();
	}
    else
    {
        CommandInterface::onButtonClick(buttonClick);
    }
}

#pragma region FightInterface::Menu::START_TURN

dpp::embed FightInterface::getStartTurnEmbed(dpp::embed& embed)
{
	Participant& participant = participants.getParticipant(participants.currentTurnOrder[0]);

	embed.set_title(getEmojiMention(participant.entity->classType->emojiName) + " " + participant.entity->name + "'s turn");
	embed.set_description("**Choose an action**");

	embed.add_field(getEmojiMention("skills") + " Skills", "Choose a skill to use", true);
	embed.add_field(getEmojiMention("inspect") + " Inspect", "Inspect an entity", true);
    embed.add_field(getEmojiMention("summon_ball") + " Catch & summon", "Catch a creature or summon one", true);
	embed.add_field(getEmojiMention("auto_play") + " Auto play", "Enable/disabled the auto play", true);
    embed.add_field(getEmojiMention("run") + " Run away", "Quit the fight", false);

	return embed;
}

std::vector<dpp::component> FightInterface::getStartTurnComponents()
{
	std::vector<dpp::component> components;
	dpp::component row;
	dpp::component skillButton = getSecondaryButton(), inspectButton = getSecondaryButton(), runButton = getSecondaryButton(),
        autoPlayButton = getSecondaryButton(), catchButton = getSecondaryButton();
	dpp::emoji* skillEmoji = getEmoji("skills");
	dpp::emoji* inspectEmoji = getEmoji("inspect");

	row.set_type(dpp::cot_action_row);

	skillButton.set_label("Skills").set_id("start_turn_skills").set_emoji(skillEmoji->name, skillEmoji->id);

    // TODO: implement these buttons
    inspectButton.set_label("Inspect").set_id("start_turn_inspect").set_emoji(inspectEmoji->name, inspectEmoji->id).set_disabled(true);
    catchButton.set_label("Catch & summon").set_id("start_turn_catch").set_disabled(true);
    autoPlayButton.set_label("Auto play").set_id("start_turn_auto_play").set_disabled(true);
    runButton.set_label("Run away").set_id("start_turn_run").set_disabled(true).set_style(dpp::cos_danger);

	row.add_component(skillButton);
	row.add_component(inspectButton);
    row.add_component(catchButton);
    row.add_component(autoPlayButton);

    components.push_back(row);

    row = dpp::component().set_type(dpp::cot_action_row);

    row.add_component(runButton);

	components.push_back(row);

	return components;
}

void FightInterface::onStartTurnButtonClick(const dpp::button_click_t& buttonClick)
{
	if (buttonClick.custom_id == "start_turn_skills")
	{
		menu = Menu::CHOOSE_SKILLS;
	}
	else if (buttonClick.custom_id == "start_turn_inspect")
	{
		menu = Menu::INSPECT_MENU;
	}
}

#pragma endregion // FightInterface::Menu::START_TURN

#pragma region FightInterface::Menu::CHOOSE_SKILLS

dpp::embed FightInterface::getChooseSkillsEmbed(dpp::embed& embed)
{
	Participant& participant = participants.getParticipant(participants.currentTurnOrder[0]);

	if (currentSkillIndex >= participant.entity->activeSkills.size() || currentSkillIndex < 0)
	{
		currentSkillIndex = 0;
	}

	auto* skill = participant.entity->activeSkills[currentSkillIndex];

	embed.add_field(skill->getCompleteName(), skill->getCompleteDescription());

	return embed;
}

std::vector<dpp::component> FightInterface::getChooseSkillsComponents()
{
	std::vector<dpp::component> components;
	dpp::component row;
	dpp::component returnButton = getSecondaryButton(), chooseTargetButton = getSecondaryButton();
    auto* targetEmoji = getEmoji("target");
    auto* returnEmoji = getEmoji("return");

	row.set_type(dpp::cot_action_row);

	returnButton.set_id("return").set_emoji(returnEmoji->name, returnEmoji->id);
	chooseTargetButton.set_label("Choose a target").set_id("choose_target").set_emoji(targetEmoji->name, targetEmoji->id).set_style(dpp::cos_success);

	row.add_component(returnButton);
	row.add_component(chooseTargetButton);

	CommandInterface::addMenuComponents(components, 0);

	components.push_back(row);

	return components;
}

void FightInterface::onChooseSkillsButtonClick(const dpp::button_click_t& buttonClick)
{
	if (buttonClick.custom_id == "return")
	{
		menu = Menu::START_TURN;
	}
	else if (buttonClick.custom_id == "choose_target")
	{
		menu = Menu::CHOOSE_TARGET;
	}
}

void FightInterface::onChooseSkillsSelect(const std::string& value)
{
	currentSkillIndex = std::stoi(value);
}

std::vector<Option> FightInterface::getChooseSkillsOptions()
{
	std::vector<Option> options;

	Participant& participant = participants.getParticipant(participants.currentTurnOrder[0]);

	int i = 0;
	for (auto* skill : participant.entity->activeSkills)
	{
		Option option;
		option.label = skill->name;
		option.value = std::to_string(i);
		option.selected = i == currentSkillIndex;

		if (skill->spellType == Enums::Type::Spell::Damage)
		{
			option.description = "Power: " + std::to_string(skill->getPower()) + " | Target: " + Enums::targetToString.at(skill->targetType);
		}
		else if (skill->spellType == Enums::Type::Spell::Status)
		{
			option.description = "Status | Target: " + Enums::targetToString.at(skill->targetType);
		}
		else if (skill->spellType == Enums::Type::Spell::Summon)
		{
			option.description = "Summon | " + Utils::replaceAll(skill->getDescription(), "*", "");
		}

		if (skill->currentCooldown > 0)
		{
			if (skill->currentCooldown <= 6)
			{
				option.emoji = getEmoji("chrono_" + std::to_string(skill->currentCooldown));
			}
			else
			{
				option.emoji = getEmoji("chrono_6");
			}
		}
		else
		{
			option.emoji = skill->getEmoji();
		}

		options.push_back(option);

		i++;
	}

	return options;
}

#pragma endregion // FightInterface::Menu::CHOOSE_SKILLS

#pragma region FightInterface::Menu::CHOOSE_TARGET

dpp::embed FightInterface::getChooseTargetEmbed(dpp::embed& embed)
{
	auto& participant = participants.getCurrentTurnParticipant();
	auto* skill = participant.entity->activeSkills[currentSkillIndex];

	embed.set_description("Choose a target for your skill");
	embed.add_field(skill->getCompleteName(), skill->getCompleteDescription());

	return embed;
}

std::vector<dpp::component> FightInterface::getChooseTargetComponents()
{
	std::vector<dpp::component> components;
	auto& participant = participants.getCurrentTurnParticipant();
	auto validTargets = participant.getAllPossibleTargets(participant.entity->activeSkills[currentSkillIndex], participants);

	participants.addParticipantsButtons(components, [&validTargets](dpp::component& button, ParticipantOrder order)
	{
		bool found = false;

		button.set_style(dpp::cos_secondary);

		for (int i = 0; i < validTargets.size(); i++)
		{
			for (int j = 0; j < validTargets[i].size(); j++)
			{
				if (validTargets[i][j].slot == order.displayOrder.slot && validTargets[i][j].team == order.displayOrder.team)
				{
					found = true;
					button.set_id("target_" + std::to_string(i) + "_" + std::to_string(j));
					break;
				}
			}
		}

		button.set_disabled(!found);

		if (order.index == -1)
		{
			auto* emoji = getEmoji("empty_spot");
			button.set_emoji(emoji->name, emoji->id);
		}
	});

    auto* returnEmoji = getEmoji("return");

	components.emplace_back(dpp::component().set_type(dpp::cot_action_row).add_component(
		dpp::component().set_type(dpp::cot_button).set_style(dpp::cos_secondary).set_id("return").set_emoji(returnEmoji->name, returnEmoji->id)
	));

	return components;
}

void FightInterface::onChooseTargetButtonClick(const dpp::button_click_t& buttonClick)
{
	if (buttonClick.custom_id == "return")
	{
		menu = Menu::CHOOSE_SKILLS;
	}
	else if (buttonClick.custom_id.starts_with("target_"))
	{
		auto& participant = participants.getCurrentTurnParticipant();
		auto* skill = participant.entity->activeSkills[currentSkillIndex];
		auto validTargets = participant.getAllPossibleTargets(skill, participants);

		int i = std::stoi(buttonClick.custom_id.substr(7, 1));

		currentTargetIndex = i;

        auto hasTargets = !participants.getParticipant(validTargets[i][0].team, validTargets[i][0].slot).entity->classType->targets.empty();

		if (skill->targetType != Enums::Type::Target::SingleEnemy || !hasTargets)
		{
			participant.selectSkill(skill);
			participant.selectTarget(validTargets[i]);

			menu = Menu::FINISH_TURN;
		}
		else
		{
			menu = Menu::CHOOSE_ENTITY_PART;
		}
	}
}

#pragma endregion // FightInterface::Menu::CHOOSE_TARGET

#pragma region FightInterface::Menu::CHOOSE_ENTITY_PART

dpp::embed FightInterface::getChooseEntityPartEmbed(dpp::embed& embed)
{
    auto& participant = participants.getCurrentTurnParticipant();
    auto validTargets = participant.getAllPossibleTargets(participant.entity->activeSkills[currentSkillIndex], participants);
    auto& targetParticipant = participants.getParticipant(validTargets[currentTargetIndex][0].team, validTargets[currentTargetIndex][0].slot);
	auto* skill = participant.entity->activeSkills[currentSkillIndex];

	embed.set_description("Select a part of the entity to target");
    embed.add_field(skill->getCompleteName(), skill->getCompleteDescription());

	for (auto* target : targetParticipant.entity->classType->targets)
	{
		if (!target->canBeUsed(participant.entity, targetParticipant.entity)) continue;

        auto emoji = getEmojiMention(target->emojiName);

        embed.add_field(emoji + " " + target->name, target->getDescription(participant.entity, targetParticipant.entity), false);
	}

	return embed;
}

std::vector<dpp::component> FightInterface::getChooseEntityPartComponents()
{
    std::vector<dpp::component> components;
    dpp::component actionRow = dpp::component().set_type(dpp::cot_action_row);

    auto& participant = participants.getCurrentTurnParticipant();
    auto validTargets = participant.getAllPossibleTargets(participant.entity->activeSkills[currentSkillIndex], participants);
    auto& targetParticipant = participants.getParticipant(validTargets[currentTargetIndex][0].team, validTargets[currentTargetIndex][0].slot);

    int i = 0;

    for (auto* target : targetParticipant.entity->classType->targets)
    {
        if (target->canBeUsed(participant.entity, targetParticipant.entity))
        {
            auto* emoji = getEmoji(target->emojiName);

            actionRow.add_component(getSecondaryButton().set_label(target->name).set_id("part_" + std::to_string(i)).set_emoji(emoji->name, emoji->id));

            if (actionRow.components.size() == 5)
            {
                components.push_back(actionRow);
                actionRow = dpp::component().set_type(dpp::cot_action_row);
            }
        }

        i++;
    }

    if (!actionRow.components.empty())
    {
        components.push_back(actionRow);
    }

    auto* returnEmoji = getEmoji("return");

    components.emplace_back(dpp::component().set_type(dpp::cot_action_row).add_component(
        getSecondaryButton().set_id("return").set_emoji(returnEmoji->name, returnEmoji->id)
    ));

	return components;
}

void FightInterface::onChooseEntityPartButtonClick(const dpp::button_click_t& buttonClick)
{
    if (buttonClick.custom_id == "return")
    {
        menu = Menu::CHOOSE_TARGET;
    }
    else if (buttonClick.custom_id.starts_with("part_"))
    {
        auto& participant = participants.getCurrentTurnParticipant();
        auto validTargets = participant.getAllPossibleTargets(participant.entity->activeSkills[currentSkillIndex], participants);
        auto& targetParticipant = participants.getParticipant(validTargets[currentTargetIndex][0].team, validTargets[currentTargetIndex][0].slot);

        int i = std::stoi(buttonClick.custom_id.substr(5));

        participant.selectSkill(participant.entity->activeSkills[currentSkillIndex]);
        participant.selectTarget(validTargets[currentTargetIndex]);
        participant.entity->state->currentTarget = targetParticipant.entity->classType->targets[i];

        menu = Menu::FINISH_TURN;
    }
}

#pragma endregion // FightInterface::Menu::CHOOSE_ENTITY_PART

#pragma region FightInterface::Menu::INSPECT_MENU

dpp::embed FightInterface::getInspectMenuEmbed(dpp::embed& embed)
{
	return dpp::embed();
}

std::vector<dpp::component> FightInterface::getInspectMenuComponents()
{
	return std::vector<dpp::component>();
}

void FightInterface::onInspectMenuButtonClick(const dpp::button_click_t& buttonClick)
{

}

#pragma endregion // FightInterface::Menu::INSPECT_MENU

#pragma region FightInterface::Menu::INSPECT_ENTITY

dpp::embed FightInterface::getInspectEntityEmbed(dpp::embed& embed)
{
	return dpp::embed();
}

std::vector<dpp::component> FightInterface::getInspectEntityComponents()
{
	return std::vector<dpp::component>();
}

void FightInterface::onInspectEntityButtonClick(const dpp::button_click_t& buttonClick)
{

}

#pragma endregion // FightInterface::Menu::INSPECT_ENTITY

#pragma region FightInterface::Menu::END_FIGHT

dpp::embed FightInterface::getEndFightEmbed(dpp::embed& embed)
{
	embed.set_description("The fight is finished!");

	return embed;
}

std::vector<dpp::component> FightInterface::getEndFightComponents()
{
	return std::vector<dpp::component>();
}

void FightInterface::onEndFightButtonClick(const dpp::button_click_t& buttonClick)
{

}

#pragma endregion // FightInterface::Menu::END_FIGHT

#pragma region Utility

std::string FightInterface::getEmojiMention(const std::string& emojiName)
{
	return getEmoji(emojiName)->get_mention();
}

dpp::emoji* FightInterface::getEmoji(const std::string& emojiName)
{
    dpp::emoji* emoji = IconManager::getEmoji(emojiName);

    if (emoji == nullptr)
    {
        emoji = IconManager::getEmoji("emoji_not_found");
    }

    return emoji;
}

dpp::component FightInterface::getSecondaryButton()
{
    return dpp::component().set_type(dpp::cot_button).set_style(dpp::cos_secondary);
}

dpp::component FightInterface::getPrimaryButton()
{
    return dpp::component().set_type(dpp::cot_button).set_style(dpp::cos_primary);
}

#pragma endregion // Utility