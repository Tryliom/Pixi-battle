#pragma once

#include <vector>
#include "skillBranch.h"

struct Skill;
struct ClassType;

struct SkillTree
{
	SkillTree(std::vector<Skill*> mainSkills, const std::vector<SkillBranch>& skillBranches, ClassType* evolution = nullptr);
	SkillTree() = default;

	std::vector<Skill*> mainSkills;
	std::vector<SkillBranch> skillBranches;
	ClassType* evolution{ nullptr };
};
