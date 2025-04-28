#include "model/entity/classType/targets/legs.h"

#include "model/entity/entity.h"
#include "model/entity/skill/skill.h"

Legs::Legs() :
	Target("Leg", Description{
		.description = "Have a 10% chance to inflict effect0 but -10% chance to have a successful hit.",
		.effects = {EffectList::SpeedDiminution}
	}, "none", 0.1f, -0.1f)
{}

void Legs::onSelectTarget(Entity* user, Entity* target, Skill* skill)
{
	this->stat.successHitChance = this->getTotalMalus();
}

void Legs::applyEffects(Entity* attacker, Entity* target, bool critical)
{
	Target::applyEffects(attacker, target, critical);

	if (target->canApplyEffect(attacker, this->getTotalBonus()))
	{
		target->applyEffect(EffectFactory::getSpeedDiminution(2));
	}
}
