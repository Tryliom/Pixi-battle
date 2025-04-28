#pragma once

#include <string>

#include "model/entity/stat.h"
#include "model/entity/classType/skin.h"
#include "model/entity/classType/target.h"
#include "model/entity/classType/skillTree.h"
#include "model/entity/classType/lineBonus.h"

struct Skill;

/**
 * The class type is associated to an entity and is used to determine his stats, skill tree, skin, etc.
 */
struct ClassType
{
	ClassType(std::string id, std::string name, std::string description, const Skin& skin, std::vector<Target*> targets, const SkillTree& skillTree,
	          const LineBonus& lineBonus, Stat baseStat, Stat bonus, Stat statGrowth);
	ClassType() = default;

	std::string id;
	std::string name;
	std::string description;
	std::string emojiName;

	Skin skin;
	std::vector<Target*> targets;
	SkillTree skillTree;
	LineBonus lineBonus;

	int xp { 0 };
	int maxXp { 50 };
	int level { 1 };

	// Used in stat growth
	int investPoints { 0 };
	// Used to improve class skills
	int skillClassPoints { 0 };

	Stat baseStat;
	Stat bonus;
	Stat statGrowth;
	Stat investedPoints;

	void updateMaxXp();

	/**
	 * Get all skills from the skill tree.
	 */
	[[nodiscard]] std::vector<Skill*> getSkills(bool onlyUnlocked = true) const;

	/**
	 * Returns all stats of the class type.
	 * @return {Stat}
	 */
	Stat getAllStats();

	friend void to_json(nlohmann::json& nlohmann_json_j, const ClassType& nlohmann_json_t);
	friend void from_json(const nlohmann::json& nlohmann_json_j, ClassType& nlohmann_json_t);
};

struct Mage : public ClassType
{
	Mage();
};

struct Thief : public ClassType
{
	Thief();
};

struct Slime : public ClassType
{
	Slime();
};
