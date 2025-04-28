#pragma once

#include <vector>
#include <string>
#include <memory>

#include "utility/enums.h"

struct Entity;
struct Skill;

enum class SkillName {
    Illumination,
    Curse,
    FireBall,
    MagicBall,
    Thorns,
    Rebirth,
    CounterExpert,
    LastRampart,
	Spit,
	DaggerThrow,
    END
};

namespace SkillFactory
{
    std::unique_ptr<Skill> getSkill(SkillName skillName, int level = 0);
}

namespace SkillManager
{
	std::vector<SkillName> getAchievementSkills();
	std::vector<SkillName> getObtainableSkills();
	std::vector<SkillName> getClassSkills();
	std::vector<SkillName> getPassiveSkills();
	std::vector<SkillName> getActiveSkills();
	std::vector<SkillName> getSkills();

	/**
	 * Give a new skill to the entity that he doesn't have yet. The skill will be added to the entity's skills. 5% chance to have a Legendary skill, 15% chance to have a Epic skill, 30% chance to have a Rare skill and 50% chance to have a Common skill.
	 * @param entity The entity to give the skill to.
	 * @param tier (optional) The tier of the skill to give.
	 */
	void obtainRandomPassiveSkill(Entity* entity, Enums::Tier tier = Enums::Tier::Unknown);

    std::unique_ptr<Skill> getSkill(const std::string& id);
}
