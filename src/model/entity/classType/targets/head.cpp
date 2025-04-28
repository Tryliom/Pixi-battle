#include "model/entity/classType/targets/head.h"

#include "model/entity/entity.h"
#include "model/entity/skill/skill.h"

Head::Head() :
	Target("Head", Description{.description = "+10% damage multiplier but -15% chance to have a successful hit."}, "none", 0.1f, -0.15f)
{}

void Head::applyEffects(Entity* attacker, Entity* target, bool critical)
{
	Target::applyEffects(attacker, target, critical);
	this->stat.damageMultiplier = this->getTotalBonus();
}

void Head::onSelectTarget(Entity* user, Entity* target, Skill* skill)
{
	this->stat.successHitChance = this->getTotalMalus();
}

