#pragma once

#include "model/entity/skill/skill.h"

struct Curse :
        public PassiveSkill
{
    explicit Curse(int level = 0);

    void onBeforeAttack(Entity* user, Entity* opponent, bool critical, bool dodge) override;
    void onEndAttack(Entity* user, Entity* opponent, bool critical, bool dodge) override;
};
