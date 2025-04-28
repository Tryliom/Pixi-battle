#pragma once

#include "model/entity/skill/skill.h"

struct Illumination :
        public PassiveSkill
{
    explicit Illumination(int level = 0);

    int currentStack;

    void onConsumeAttack(Entity* user) override;

    void onEndFight(Entity* user) override;

    void updateStacks();
};
