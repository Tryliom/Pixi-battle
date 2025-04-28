#include "model/entity/effect/effects/shield.h"

#include "model/entity/entity.h"
#include "model/entity/fightState.h"

Shield::Shield(int turns, float value) :
        Effect("Shield", "shield", "Can block a certain amount of damage for the user", EffectType::Buff, EffectUpdateTurn::End, false, turns)
{
    this->value = value;
}

void Shield::onStartDamage(Entity* user, Entity* opponent, float& damage, Enums::Type::Damage damageType, float& damageReductionFlat, float& damageReductionPercent)
{
    if (this->value - damage < 0)
    {
	    damage -= this->value;
        this->value = 0;
    }
    else
    {
        this->value -= damage;
	    damage = 0.f;
    }

    user->state->displayStatus(Status::AbsorbShield());
}

bool Shield::isExpired()
{
    return Effect::isExpired() || this->value <= 0.f;
}

bool Shield::isBetter(Effect* effect)
{
    auto* effectShield = dynamic_cast<Shield*>(effect);
    return this->value > effectShield->value;
}