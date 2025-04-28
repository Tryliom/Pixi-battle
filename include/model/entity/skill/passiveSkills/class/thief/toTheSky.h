#pragma once

#include "model/entity/skill/skill.h"

struct ToTheSky :
        public PassiveSkill
{
    explicit ToTheSky(int level = 0);

    void onBeforeAttack(Entity* user, Entity* opponent, bool critical, bool dodge) override;
    void onEndFight(Entity* user) override;
};