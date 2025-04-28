#pragma once

#include <string>
#include <vector>

#include "factory/effectFactory.h"

struct SkillParameter;
struct Skill;

struct Description {
	std::string description;
	std::vector<SkillParameter> parameters;
	std::vector<EffectList> effects;

	std::string asString(Skill* skill = nullptr);
};
