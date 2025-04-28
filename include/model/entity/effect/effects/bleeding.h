#pragma once

#include "model/entity/effect/effect.h"

struct Bleeding :
        public Effect
{
    explicit Bleeding(int turns = 0, Entity* attacker = nullptr);

    float damage;

    void onStartTurn(Entity* entity) override;
};
