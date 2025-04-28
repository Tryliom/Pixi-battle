#pragma once

#include <utility>
#include <vector>

struct Skill;
struct ClassType;

struct SkillBranch
{
	SkillBranch(Skill* mainSkill, std::vector<Skill*> skills, ClassType* evolution = nullptr);

	Skill* mainSkill;
	std::vector<Skill*> skills;
	ClassType* evolution;
};
