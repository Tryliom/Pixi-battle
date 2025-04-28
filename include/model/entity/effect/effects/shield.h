#pragma once

#include "model/entity/effect/effect.h"

struct Shield : public Effect
{
	explicit Shield(int turns = 0, float value = 0);

	float value;

	void onStartDamage(Entity* user, Entity* opponent, float& damage, Enums::Type::Damage damageType, float& damageReductionFlat, float& damageReductionPercent) override;
	bool isExpired() override;
	bool isBetter(Effect* effect) override;
};
