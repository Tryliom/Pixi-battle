#include "model/entity/classType/targets/chest.h"

#include "model/entity/entity.h"
#include "model/entity/skill/skill.h"

Chest::Chest() :
	Target("Chest", Description{.description = "+15% to have a successful hit but -5% for damage multiplier."}, "none", 0.15f, -0.05f)
{}

void Chest::applyEffects(Entity* attacker, Entity* target, bool critical)
{
	Target::applyEffects(attacker, target, critical);
	this->stat.damageMultiplier = this->getTotalMalus();
}

void Chest::onSelectTarget(Entity* user, Entity* target, Skill* skill)
{
	this->stat.successHitChance = this->getTotalBonus();
}
