#include "model/entity/classType/targets/back.h"

#include "model/entity/effect/effects/stealth.h"
#include "model/entity/entity.h"
#include "model/entity/skill/skill.h"
#include "model/entity/fightState.h"

Back::Back() :
	Target("Back", Description{.description = "+15% defense penetration."}, "none", 0.15f, -0.05f)
{}

bool Back::canBeUsed(Entity* user, Entity* target)
{
	return user->state->hasEffect<Stealth>();
}

void Back::applyEffects(Entity* attacker, Entity* target, bool critical)
{
	Target::applyEffects(attacker, target, critical);

	this->stat.magicalArmorPenetrationPercent = this->getTotalBonus();
	this->stat.physicalArmorPenetrationPercent = this->getTotalBonus();
}
