#include "model/entity/effect/effects/poison.h"

#include "model/entity/entity.h"
#include "model/entity/fightState.h"

Poison::Poison(int turns) :
	Effect("Poison", "poison", "Inflict 5% max health of the user per turns", EffectType::Debuff, EffectUpdateTurn::Start, turns, true)
{}

void Poison::onStartTurn(Entity* entity)
{
	float damage = entity->calculateDamageTaken(entity->state->maxHp * 0.05f, Enums::Type::Damage::True, 0, 0);
	entity->takeDamage(damage, Enums::Type::Damage::True);

	entity->state->displayStatus(Status::Poison());

	Effect::onStartTurn(entity);
}