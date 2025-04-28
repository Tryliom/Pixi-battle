#pragma once

#include "model/entity/effect/effect.h"

struct Stealth : public Effect
{
	explicit Stealth(int turns = 0);

	bool active;

	void onEndDamage(Entity* user, Entity* opponent, float hp, Enums::Type::Damage damageType) override;
};
