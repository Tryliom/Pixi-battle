#include "model/entity/classType/targets/arms.h"

#include "model/entity/entity.h"
#include "model/entity/skill/skill.h"

Arms::Arms() :
	Target("Arm", Description{
		.description = "Have a 10% chance to inflict effect0 for 2 turns but -10% chance to have a successful hit.",
		.effects = {EffectList::AttackDiminution}
	}, "none", 0.1f, -0.1f)
{}

void Arms::applyEffects(Entity* attacker, Entity* target, bool critical)
{
	Target::applyEffects(attacker, target, critical);

	if (target->canApplyEffect(attacker, getTotalBonus()))
	{
		target->applyEffect(EffectFactory::getAttackDiminution(2));
	}
}

void Arms::onSelectTarget(Entity* user, Entity* target, Skill* skill)
{
	this->stat.successHitChance = getTotalMalus();
}
