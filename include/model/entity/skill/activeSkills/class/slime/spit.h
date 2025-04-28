#pragma once

#include "model/entity/skill/skill.h"

struct Spit : public ActiveSkill
{
	explicit Spit(int level = 0);

	void useDamageSkill(Participant& user, Participant& target, bool critical, bool dodge) override;
};