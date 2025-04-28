#include "model/entity/classType/skillTree.h"

#include <utility>

#include "model/entity/skill/skill.h"
#include "model/entity/classType/skillBranch.h"

SkillTree::SkillTree(std::vector<Skill*> mainSkills, const std::vector<SkillBranch>& skillBranches, ClassType* evolution)
{
	this->mainSkills = std::move(mainSkills);
	this->skillBranches = skillBranches;
	this->evolution = evolution;
}