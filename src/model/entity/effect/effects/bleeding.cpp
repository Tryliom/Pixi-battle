#include "model/entity/effect/effects/bleeding.h"

#include "utility/enums.h"
#include "model/entity/entity.h"
#include "model/entity/fightState.h"

Bleeding::Bleeding(int turns, Entity* attacker) :
        Effect("Bleeding", "bleeding", "Deal physical damage equivalent to 25% of physical attack of the attacker that ignore 30% of the defense and reduce the heal multiplier by 5%.",
               EffectType::Debuff, EffectUpdateTurn::Start, true, turns, Stat{.healMultiplier = -0.05})
{
    if (attacker != nullptr)
    {
        this->damage = attacker->stats.physicalAttack * 0.25f;
    }
}

void Bleeding::onStartTurn(Entity* entity)
{
    float totalDamage = entity->calculateDamageTaken(this->damage, Enums::Type::Damage::Physical, 0, 0.3f);
    entity->takeDamage(totalDamage, Enums::Type::Damage::Physical);

	entity->state->displayStatus(Status::Bleeding());

    Effect::onStartTurn(entity);
}