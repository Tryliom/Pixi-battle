#pragma once

#include "model/entity/effect/effect.h"

struct Burn : public Effect
{
    explicit Burn(int turns = 0, Entity* attacker = nullptr);

    float damage;

    void onStartTurn(Entity* entity) override;
};
