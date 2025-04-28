#pragma once

#include "model/entity/classType/target.h"

struct Back : public Target
{
	Back();

	//TODO: Remove stealth effect after the attack.
	bool canBeUsed(Entity* user, Entity* target) override;
	void applyEffects(Entity* attacker, Entity* target, bool critical) override;
};