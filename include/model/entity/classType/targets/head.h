#pragma once

#include "model/entity/classType/target.h"

struct Head : Target
{
	Head();

	void onSelectTarget(Entity* user, Entity* target, Skill* skill) override;
	void applyEffects(Entity* attacker, Entity* target, bool critical) override;
};
