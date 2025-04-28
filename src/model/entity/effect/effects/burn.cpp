#include "model/entity/effect/effects/burn.h"

#include "model/entity/entity.h"
#include "model/entity/fightState.h"

Burn::Burn(int turns, Entity* attacker) :
        Effect("Burn", "burn", "Deal magical damage equivalent to 40% of magic attack of the attacker that ignore 40% of the defense.",
               EffectType::Debuff, EffectUpdateTurn::Start, true, turns)
{
    if (attacker != nullptr)
    {
        this->damage = attacker->stats.magicalAttack * 0.4f;
    }
}

void Burn::onStartTurn(Entity* entity)
{
	float totalDamage = entity->calculateDamageTaken(this->damage, Enums::Type::Damage::Magical, 0, 0.4f);
    entity->takeDamage(totalDamage, Enums::Type::Damage::Magical);

	entity->state->displayStatus(Status::Burn());

    Effect::onStartTurn(entity);
}