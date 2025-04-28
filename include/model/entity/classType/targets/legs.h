#pragma once

#include "model/entity/classType/target.h"

struct Legs : public Target
{
	Legs();

	void onSelectTarget(Entity* user, Entity* target, Skill* skill) override;
	void applyEffects(Entity* attacker, Entity* target, bool critical) override;
};

