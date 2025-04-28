#include "model/entity/classType/skillBranch.h"

#include "model/entity/skill/skill.h"

SkillBranch::SkillBranch(Skill* mainSkill, std::vector<Skill*> skills, ClassType* evolution)
{
	this->mainSkill = mainSkill;
	this->skills = std::move(skills);
	this->evolution = evolution;
}
