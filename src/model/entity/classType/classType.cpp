#include "model/entity/classType/classType.h"

#include "manager/classManager.h"

#include "model/entity/classType/targets/head.h"
#include "model/entity/classType/targets/chest.h"
#include "model/entity/classType/targets/legs.h"
#include "model/entity/classType/targets/arms.h"
#include "model/entity/classType/targets/back.h"

#include "model/entity/skill/activeSkills/class/thief/daggerThrow.h"
#include "model/entity/skill/passiveSkills/class/thief/thiefsLife.h"
#include "model/entity/skill/activeSkills/class/thief/smokeScreen.h"
#include "model/entity/skill/activeSkills/class/thief/stab.h"
#include "model/entity/skill/passiveSkills/class/thief/toTheSky.h"

#include "model/entity/skill/activeSkills/class/mage/magicBall.h"
#include "model/entity/skill/activeSkills/class/mage/fireBall.h"
#include "model/entity/skill/passiveSkills/class/mage/curse.h"
#include "model/entity/skill/passiveSkills/class/mage/illumination.h"
#include "model/entity/classType/skillBranch.h"
#include "model/entity/skill/activeSkills/class/slime/spit.h"

ClassType::ClassType(std::string id, std::string name, std::string description, const Skin& skin, std::vector<Target*> targets, const SkillTree& skillTree,
                     const LineBonus& lineBonus, Stat baseStat, Stat bonus, Stat statGrowth)
{
	this->id = std::move(id);
	this->name = std::move(name);
	this->description = std::move(description);
	this->emojiName = Utils::toLowerCase(std::string(this->name));
	this->skin = skin;
	this->targets = std::move(targets);
	this->skillTree = skillTree;
	this->lineBonus = lineBonus;
	this->baseStat = baseStat;
	this->bonus = bonus;
	this->statGrowth = statGrowth;
}

std::vector<Skill*> ClassType::getSkills(bool onlyUnlocked) const
{
	std::vector<Skill*> skills;
	for (Skill* skill: this->skillTree.mainSkills)
	{
		skills.emplace_back(skill);
	}
	for (const SkillBranch& skillBranch: this->skillTree.skillBranches)
	{
		if (skillBranch.mainSkill->level >= 0 || !onlyUnlocked)
		{
			skills.emplace_back(skillBranch.mainSkill);

			for (Skill* skill: skillBranch.skills)
			{
				if (skill->level >= 0 || !onlyUnlocked)
				{
					skills.emplace_back(skill);
				}
			}
		}
	}
	return skills;
}

Stat ClassType::getAllStats()
{
	return StatUtility::addition({
		                             this->baseStat,
		                             this->bonus,
		                             StatUtility::multiply(this->statGrowth, static_cast<float>(level)),
		                             this->investedPoints * this->statGrowth
	                             });
}

void ClassType::updateMaxXp()
{
	int futureMaxXp = 50;

	for (int i = 1; i < this->level; i++)
	{
		futureMaxXp += 15 * (int) i / 10;
	}

	this->maxXp = futureMaxXp;
}

void from_json(const json& nlohmann_json_j, ClassType& nlohmann_json_t)
{
	std::string id = nlohmann_json_j["id"];
	ClassType* classType = ClassManager::getClass(id);

	if (classType == nullptr)
	{
		return;
	}

	nlohmann_json_t.xp = nlohmann_json_j["xp"];
	nlohmann_json_t.maxXp = nlohmann_json_j["maxXp"];
	nlohmann_json_t.level = nlohmann_json_j["level"];
	nlohmann_json_t.skillClassPoints = nlohmann_json_j["skillClassPoints"];
	nlohmann_json_t.investPoints = nlohmann_json_j["investPoints"];
	nlohmann_json_t = *classType;
	nlohmann_json_j.at("investedPoints").get_to(nlohmann_json_t.investedPoints);

	std::vector<Skill*> skills = nlohmann_json_t.getSkills();

	for (Skill skill: nlohmann_json_j["skills"])
	{
		for (Skill* skillPtr: skills)
		{
			if (skill.id == skillPtr->id)
			{
				skillPtr->level = skill.level;

				for (SkillImprovement skillImprovement: skillPtr->improvementsPerLevel)
				{
					if (skillImprovement.improvement == Enums::Type::Improvement::Choice)
					{
						skillImprovement.choiceIndex = skill.improvementsPerLevel[4].choiceIndex;
					}
				}

				continue;
			}
		}
	}
}

void to_json(json& nlohmann_json_j, const ClassType& nlohmann_json_t)
{
	nlohmann_json_j["id"] = nlohmann_json_t.id;
	nlohmann_json_j["xp"] = nlohmann_json_t.xp;
	nlohmann_json_j["maxXp"] = nlohmann_json_t.maxXp;
	nlohmann_json_j["level"] = nlohmann_json_t.level;
	nlohmann_json_j["investPoints"] = nlohmann_json_t.investPoints;
	nlohmann_json_j["skillClassPoints"] = nlohmann_json_t.skillClassPoints;
	nlohmann_json_j["investedPoints"] = nlohmann_json_t.investedPoints;
	json skills;

	for (Skill* skill: nlohmann_json_t.getSkills())
	{
		skills.emplace_back(*skill);
	}

	nlohmann_json_j["skills"] = skills;
}

Mage::Mage() :
	ClassType("85ed3ff2-8f6d-4c6e-b9f6-670298e1ea2a", "Mage", "A class that deal damage on a single target with debuff and some healing",
	          Skin(
		          SkinAnimation("default", "default", Enums::Type::Animation::Idle),
		          SkinAnimation("default", "default", Enums::Type::Animation::Apparition),
		          SkinAnimation("default", "default", Enums::Type::Animation::Hit),
		          SkinAnimation("default", "default", Enums::Type::Animation::Death),
		          SkinAnimation("default", "default", Enums::Type::Animation::Attack),
		          true
	          ),
	          {
		          new Head(),
		          new Chest(),
		          new Legs(),
		          new Arms(),
		          new Back()
	          },
	          SkillTree(
		          {
			          new MagicBall(),
			          new Illumination()
		          },
		          {
			          SkillBranch(
				          new FireBall(-1),
				          {new Curse(-1)}
			          )
		          }
	          ),
	          LineBonus(
		          Enums::Type::Line::Back, Stat{.finalMagicalAttackPercent = 0.1}
	          ),
	          Stat{
		          .physicalAttackFlat = 10,
		          .magicalAttackFlat = 30,
		          .healthFlat = 200,
		          .speedFlat = 50,
		          .physicalArmorFlat = 8,
		          .magicalArmorFlat = 12,
		          .criticalChance = 0.15,
		          .criticalDamage = 1,
		          .dodgeChance = 0.1
	          },
	          Stat{
		          .magicalAttackPercent = 0.1,
	          },
	          Stat{
		          .magicalAttackFlat = 3,
		          .healthFlat = 15,
		          .speedFlat = 2.f / 5.f,
		          .physicalArmorFlat = 1,
		          .magicalArmorFlat = 2
	          }
	)
{}

Thief::Thief() :
	ClassType("835eb033-e15b-4928-9780-5df9ebb607ba", "Thief", "A class that deal damage on a single target with high attackBarPercentage and low health",
	          Skin(
		          SkinAnimation("default", "default", Enums::Type::Animation::Idle),
		          SkinAnimation("default", "default", Enums::Type::Animation::Apparition),
		          SkinAnimation("default", "default", Enums::Type::Animation::Hit),
		          SkinAnimation("default", "default", Enums::Type::Animation::Death),
		          SkinAnimation("default", "default", Enums::Type::Animation::Attack),
		          true
	          ),
	          {
		          new Head(),
		          new Chest(),
		          new Legs(),
		          new Arms(),
		          new Back()
	          },
	          SkillTree(
		          {
			          new DaggerThrow(),
			          new ThiefsLife(),
			          new SmokeScreen()
		          },
		          {
			          SkillBranch(
				          new Stab(-1),
				          {new ToTheSky(-1)}
			          )
		          }
	          ),
	          LineBonus(
		          Enums::Type::Line::Back, Stat{.finalPhysicalAttackPercent = 0.1}
	          ),
	          Stat{
		          .physicalAttackFlat = 30,
		          .magicalAttackFlat = 10,
		          .healthFlat = 150,
		          .speedFlat = 60,
		          .physicalArmorFlat = 5,
		          .magicalArmorFlat = 5,
		          .criticalChance = 0.2,
		          .criticalDamage = 1,
		          .dodgeChance = 0.15
	          },
	          Stat{
		          .physicalAttackPercent = 0.05,
		          .speedPercent = 0.05
	          },
	          Stat{
		          .physicalAttackFlat = 4,
		          .healthFlat = 10,
		          .speedFlat = 3.f / 5.f,
		          .physicalArmorFlat = 1,
		          .magicalArmorFlat = 1
	          }
	)
{}

Slime::Slime() :
	ClassType("fbf03b84-03ed-49a4-913f-8f2cb0991698", "Slime", "Have you seen Limule recently ?",
	          Skin(
		          SkinAnimation("slime", "default", Enums::Type::Animation::Idle),
		          SkinAnimation("slime", "default", Enums::Type::Animation::Apparition),
		          SkinAnimation("slime", "default", Enums::Type::Animation::Hit),
		          SkinAnimation("slime", "default", Enums::Type::Animation::Death),
		          SkinAnimation("slime", "default", Enums::Type::Animation::Attack),
		          true
	          ),
	          {
		          new Head(),
		          new Chest(),
		          new Back()
	          },
	          SkillTree(
		          {
			          new Spit(),
		          },
		          {

		          }
	          ),
	          LineBonus(
		          Enums::Type::Line::Front, Stat{.finalHealthPercent = 0.1}
	          ),
	          Stat{
		          .physicalAttackFlat = 10,
		          .magicalAttackFlat = 10,
		          .healthFlat = 230,
		          .speedFlat = 40,
		          .physicalArmorFlat = 15,
		          .magicalArmorFlat = 15,
		          .criticalChance = 0.15,
		          .criticalDamage = 1,
		          .dodgeChance = 0.1
	          },
	          Stat{
		          .healthPercent = 0.1
	          },
	          Stat{
		          .physicalAttackFlat = 1,
		          .magicalAttackFlat = 1,
		          .healthFlat = 20,
		          .speedFlat = 1.f / 5.f,
		          .physicalArmorFlat = 3,
		          .magicalArmorFlat = 3
	          }
	)
{}