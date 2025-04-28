#pragma once

#include "model/entity/skill/skill.h"

struct ThiefsLife :
        public PassiveSkill
{
    explicit ThiefsLife(int level = 0);

    void onEndUpdateHP(Entity* user, float hp) override;
    void onEndFight(Entity* user) override;
};
