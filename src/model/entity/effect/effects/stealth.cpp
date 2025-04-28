#include "model/entity/effect/effects/stealth.h"

Stealth::Stealth(int turns) :
	Effect("Stealth", "stealth", "The user cannot be targeted by a single target attack if there is more than one alive ally. Dispelled when taking damage. Reduce taken damage by 50%.",
	       EffectType::Buff, EffectUpdateTurn::End, false, turns, Stat{.damageTakenMultiplier = -0.5f})
{
	this->active = true;
}

void Stealth::onEndDamage(Entity* user, Entity* opponent, float hp, Enums::Type::Damage damageType)
{
	if (hp < 0)
	{
		this->active = false;
	}
}

