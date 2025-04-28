#include "model/entity/skill/skill.h"

#include "manager/skillManager.h"
#include "manager/iconManager.h"
#include "model/entity/entity.h"
#include "model/entity/fightState.h"

#include <utility>

std::string SkillImprovement::getDescription()
{
    if (this->improvement == Enums::Type::Improvement::Power)
    {
        return "Increases the power by " + std::to_string(this->value);
    }
    else if (this->improvement == Enums::Type::Improvement::Cooldown)
    {
        return "Decrease the cooldown by " + std::to_string(this->value) + " turn" + (this->value > 1 ? "s" : "");
    }
    else if (this->improvement == Enums::Type::Improvement::Stat)
    {
        return "Gains " + StatUtility::formatStat(this->stat);
    }
    else if (this->improvement == Enums::Type::Improvement::Choice)
    {
        std::string choicesDescription;
        for (int i = 0; i < this->choices.size(); i++)
        {
            SkillImprovement* choice = &this->choices[i];
            choicesDescription += std::to_string(i + 1) + ": " + choice->description.asString() + "\n";
        }
        return "Choose one of the following improvements:\n" + choicesDescription;
    }
    else if (this->improvement == Enums::Type::Improvement::EffectChance)
    {
        return "Increase the chance of inflicting the effect by " + std::to_string(this->value) + "%";
    }
    else if (this->improvement == Enums::Type::Improvement::TurnEffect)
    {
        return "Increase the duration of the effect by " + std::to_string(this->value) + " turn" + (this->value > 1 ? "s" : "");
    }
    else if (this->improvement == Enums::Type::Improvement::DamagePercent)
    {
        return "Increase the damage by " + std::to_string(this->value) + "%";
    }
    else if (this->improvement == Enums::Type::Improvement::Special)
    {
        return this->description.asString();
    }
    else if (this->improvement == Enums::Type::Improvement::MaxStack)
    {
        return "Increase the maximum stack by " + std::to_string(this->value);
    }
    else if (this->improvement == Enums::Type::Improvement::Stack)
    {
        return "Increase the stack by " + std::to_string(this->value);
    }
    else if (this->improvement == Enums::Type::Improvement::Percent)
    {
        return "Increase the percentage by " + std::to_string(this->value) + "%";
    }
    else
    {
        return "";
    }
}

SkillParameter::SkillParameter(Enums::Type::Improvement type, int defaultValue)
{
    this->type = type;
    this->defaultValue = defaultValue;
}

Skill::Skill(std::string id, std::string name, Description description, std::string emojiName, Enums::Tier tier, int level, std::vector<SkillImprovement> improvementsPerLevel, bool onlySummon)
{
    this->id = std::move(id);
    this->name = std::move(name);
    this->description = std::move(description);
    this->emojiName = std::move(emojiName);
    this->tier = tier;
    this->level = level;
    this->improvementsPerLevel = std::move(improvementsPerLevel);
    this->onlySummon = onlySummon;
}

Skill::Skill()
{}

std::string Skill::formatSkillName() const
{
    dpp::emoji* emoji = IconManager::getEmoji(this->emojiName);
    std::string skillName;
    if (emoji != nullptr)
    {
        skillName += emoji->get_mention() + " ";
    }
    skillName += this->name;
    if (this->level > 0)
    {
        skillName += " +" + std::to_string(this->level);
    }
    return skillName;
}

std::string Skill::getExtraDescriptions() const
{
    std::string desc;
    for (int i = 0; i < this->improvementsPerLevel.size(); i++)
    {
        if ((i + 1) <= this->level)
        {
            SkillImprovement improvement = this->improvementsPerLevel[i];

            if (improvement.improvement == Enums::Type::Improvement::Choice && improvement.choiceIndex != -1)
            {
                desc += improvement.choices[improvement.choiceIndex].getDescription() + ". ";
            }
            else if (improvement.improvement == Enums::Type::Improvement::Special)
            {
                desc += improvement.getDescription() + ". ";
            }
        }
    }

    return desc;
}

std::string Skill::getDescription()
{
    std::string extraDescriptions = this->getExtraDescriptions();
    std::string desc = this->description.asString(this);

    if (extraDescriptions.length() > 0)
    {
        return desc + " " + extraDescriptions;
    }
    else
    {
        return desc;
    }
}

int Skill::getMaxLevel() const
{
    return (int) this->improvementsPerLevel.size();
}

Stat Skill::getAllStats() const
{
    std::vector<Stat> stats = {};

    int i = 1;
    for (const SkillImprovement& improvement: this->improvementsPerLevel)
    {
        if (i <= this->level)
        {
            stats.emplace_back(improvement.stat);
        }
        i++;
    }

    return StatUtility::addition(stats);
}

void Skill::addTargetExtraDescriptionAsUser(Target* target, std::string& desc)
{}

void Skill::addTargetExtraDescriptionAsTarget(Target* target, std::string& desc)
{}

std::vector<Skill> Skill::getActiveSkills(Entity* user)
{
    return {};
}

float Skill::getImprovementTotalValue(Enums::Type::Improvement improvement) const
{
	using namespace Enums::Type;
    auto totalValue = static_cast<float>(getImprovementTotalValueForDisplay(improvement));
	std::vector<Improvement> percentImprovements =
	{
		Improvement::DamagePercent, Improvement::EffectChance, Improvement::Percent
	};

	if (std::find(percentImprovements.begin(), percentImprovements.end(), improvement) != percentImprovements.end())
	{
		totalValue /= 100.f;
	}

	return totalValue;
}

int Skill::getImprovementTotalValueForDisplay(Enums::Type::Improvement improvement) const
{
	int totalValue = 0;
	int i = 1;

	for (const SkillImprovement& improvementPerLevel: this->improvementsPerLevel)
	{
		if (improvementPerLevel.improvement == improvement && i <= this->level)
		{
			totalValue += improvementPerLevel.value;
		}
		i++;
	}

	for (SkillParameter parameter: this->description.parameters)
	{
		if (parameter.type == improvement)
		{
			totalValue += parameter.defaultValue;
		}
	}

	return totalValue;
}

int Skill::getCostReduction(Skill* attack)
{
    return 0;
}

void to_json(json& nlohmann_json_j, const Skill& nlohmann_json_t)
{
    nlohmann_json_j["id"] = nlohmann_json_t.id;
    nlohmann_json_j["level"] = nlohmann_json_t.level;
    nlohmann_json_j["choice"] = -1;
    for (const SkillImprovement& improvement: nlohmann_json_t.improvementsPerLevel)
    {
        if (improvement.improvement == Enums::Type::Improvement::Choice)
        {
            nlohmann_json_j["choice"] = improvement.choiceIndex;
        }
    }
}

void from_json(const json& nlohmann_json_j, Skill& nlohmann_json_t)
{
    std::unique_ptr<Skill> skill = SkillManager::getSkill(nlohmann_json_j["id"]);

    if (skill == nullptr)
    {
        nlohmann_json_t.id = "null";
    }
    else
    {
        nlohmann_json_t = *skill;
        nlohmann_json_t.level = nlohmann_json_j["level"];

        for (SkillImprovement& improvement: nlohmann_json_t.improvementsPerLevel)
        {
            if (improvement.improvement == Enums::Type::Improvement::Choice)
            {
                improvement.choiceIndex = nlohmann_json_j["choice"];
            }
        }
    }
}

dpp::emoji* Skill::getEmoji() const
{
	dpp::emoji* emoji = IconManager::getEmoji(this->emojiName);

	if (emoji == nullptr)
	{
		emoji = IconManager::getEmoji("default");
	}

	return emoji;
}

std::string Skill::getCompleteName() const
{
	return getEmoji()->get_mention() + " " + this->name;
}

PassiveSkill::PassiveSkill(std::string id, std::string name, Description description, std::string emojiName, Enums::Tier tier, int level, std::vector<SkillImprovement> improvementsPerLevel, Stat userStat, Stat allyStat, Stat opponentStat, bool onlySummon)
        :
        Skill(std::move(id), std::move(name), std::move(description), std::move(emojiName), tier, level, std::move(improvementsPerLevel), onlySummon)
{
    this->userStat = userStat;
    this->allyStat = allyStat;
    this->opponentStat = opponentStat;
}

Stat PassiveSkill::getAllStats() const
{
    return StatUtility::addition({this->userStat, Skill::getAllStats()});
}

Stat PassiveSkill::getAllyStat(Entity* ally) const
{
    return this->allyStat;
}

Stat PassiveSkill::getOpponentStat(Entity* opponent) const
{
    return this->opponentStat;
}

std::string PassiveSkill::getCompleteDescription()
{
	std::string description;

	description += "\n\n_" + this->description.asString(this);

	// Add to the description all skillsImprovement that are "Special"
	int i = 0;
	for (auto& skillImprovement : this->improvementsPerLevel)
	{
		if (skillImprovement.improvement == Enums::Type::Improvement::Special && this->level >= i)
		{
			description += "\n" + skillImprovement.description.asString();
		}

		i++;
	}

	description += "_\n\n";

	// display all effect used in description
	for (auto& effect : this->description.effects)
	{
		EffectData effectData = EffectFactory::getEffectData(effect);
		dpp::emoji* emoji = IconManager::getEmoji(effectData.emojiName);

		if (emoji == nullptr)
		{
			emoji = IconManager::getEmoji("default");
		}

		description += "\n" + emoji->get_mention() + " " + effectData.name + ": _" + effectData.description + "_";
	}

	return description;
}

ActiveSkill::ActiveSkill(std::string id, std::string name, Description description, std::string emojiName, Enums::Tier tier, int level, std::vector<SkillImprovement> improvementsPerLevel,
                         int power, int cost, int cooldown, Enums::Type::Cost costType, Enums::Type::Damage damageType, Enums::Type::Target targetType,
                         Enums::Type::Spell spellType, const SkillAnimation& animation, bool onlySummon) :
        Skill(std::move(id), std::move(name), std::move(description), std::move(emojiName), tier, level, std::move(improvementsPerLevel), onlySummon)
{
    this->power = power;
    this->cost = cost;
    this->cooldown = cooldown;
    this->currentCooldown = 0;
    this->costType = costType;
    this->damageType = damageType;
    this->targetType = targetType;
    this->spellType = spellType;
    this->animation = animation;
}

int ActiveSkill::getPower()
{
    return this->power + this->getImprovementTotalValueForDisplay(Enums::Type::Improvement::Power);
}

int ActiveSkill::getCost() const
{
    return this->cost;
}

int ActiveSkill::getCooldown()
{
    return this->cooldown - this->getImprovementTotalValueForDisplay(Enums::Type::Improvement::Cooldown);
}

void ActiveSkill::use(Participants& participants, Participant& user, const std::vector<ParticipantTarget>& targets)
{
	if (this->spellType == Enums::Type::Spell::Damage)
	{
		for (ParticipantTarget target: targets)
		{
			Participant& participant = participants.getParticipant(target.team, target.slot);

			bool dodge = participant.entity->canDodge(user.entity);
			bool critical = !dodge && participant.entity->canCritical(user.entity);

			this->useDamageSkill(user, participant, critical, dodge);
			participant.entity->onReceiveAttack(user.entity, this, critical, dodge);
			participant.animation.updateStatus(Enums::Type::Animation::Hit);

			if (dodge)
			{
				user.entity->onMissedAttack(participant.entity);
				participant.entity->onOpponentMissedAttack(user.entity);
			}
			else if (critical)
			{
				participant.entity->onOpponentCritical(user.entity);
			}
		}
	}
	else if (this->spellType == Enums::Type::Spell::Status)
	{
		for (ParticipantTarget target: targets)
		{
			Participant& participant = participants.getParticipant(target.team, target.slot);

			this->useStatusSkill(user, participant);
		}
	}
	else if (this->spellType == Enums::Type::Spell::Summon)
	{
		for (ParticipantTarget target: targets)
		{
			this->useSummonSkill(user, target);
		}
	}

	this->consumeAttack(user);
}

void ActiveSkill::useDamageSkill(Participant& user, Participant& target, bool critical, bool dodge)
{
	float damage = user.entity->calculateSkillDamage(
		this->damageType,
		target.entity,
		critical,
		dodge,
		this->getPower()
	);

	user.entity->attack(target.entity, damage, this->damageType, critical, dodge);
}

void ActiveSkill::useStatusSkill(Participant& user, Participant& target) {}

void ActiveSkill::useSummonSkill(Participant& user, ParticipantTarget target) {}

void ActiveSkill::consumeAttack(Participant& user)
{
	if (this->currentCooldown > 0) return;

	float finalCost = this->getCost() * (1.f - this->getCostReduction(this));

	if (this->costType == Enums::Type::Cost::Health)
	{
		// This is health percentage
		if (finalCost > 1.f)
		{
			finalCost = user.entity->state->maxHp * (finalCost / 100.f);
		}

		user.entity->takeDamage(finalCost, Enums::Type::Damage::True);
	}

	this->currentCooldown = this->getCooldown() + 1;

	user.entity->consumeAttack();

    user.chosenSkill = nullptr;
    user.targets.clear();
}

bool ActiveSkill::canUse(Entity* user)
{
    return this->hasCost(user) && this->currentCooldown == 0;
}

bool ActiveSkill::hasCost(Entity* user)
{
    float finalCost = this->cost * (1.f - user->getCostReduction(this));

    if (this->costType != Enums::Type::Cost::None && finalCost > 0)
    {
        if (this->costType == Enums::Type::Cost::Health)
            return user->state->hp >= finalCost;
    }

    return true;
}

void ActiveSkill::onStartFight(Entity* user, Enums::Type::Line line)
{
    this->currentCooldown = 0;
}

void ActiveSkill::onEndFight(Entity* user)
{
    this->currentCooldown = 0;
}

void ActiveSkill::onEndTurn(Entity* user, Skill* attackUsed)
{
    if (this->currentCooldown > 0)
        this->currentCooldown--;
}

std::string ActiveSkill::getCompleteDescription()
{
	std::string description;

	description += "Power: " + (this->power <= 0 ? "-" : std::to_string(this->power));
	description += "\nTarget type: " + Enums::targetToString[this->targetType];

	if (this->damageType != Enums::Type::Damage::None)
	{
		description += "\nDamage type: " + Enums::damageToString[this->damageType];
	}

	if (this->cooldown > 0)
	{
		description += "\nCooldown: " + std::to_string(this->cooldown) + " turns";
	}

	description += "\n\n_" + this->description.asString(this);

	// Add to the description all skillsImprovement that are "Special"
	int i = 0;
	for (auto& skillImprovement : this->improvementsPerLevel)
	{
		if (skillImprovement.improvement == Enums::Type::Improvement::Special && this->level >= i)
		{
			description += "\n" + skillImprovement.description.asString();
		}

		i++;
	}

	description += "_\n\n";

	// display all effect used in description
	for (auto& effect : this->description.effects)
	{
		EffectData effectData = EffectFactory::getEffectData(effect);
		dpp::emoji* emoji = IconManager::getEmoji(effectData.emojiName);

		if (emoji == nullptr)
		{
			emoji = IconManager::getEmoji("default");
		}

		description += "\n" + emoji->get_mention() + " " + effectData.name + ": _" + effectData.description + "_";
	}

	return description;
}
