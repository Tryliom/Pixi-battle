#pragma once

#include "model/entity/skill/skill.h"

struct LastRampart :
        public PassiveSkill
{
    explicit LastRampart(int level = 0);

    bool canUse;

    void onEndDamage(Entity* user, Entity* opponent, float hp, Enums::Type::Damage damageType) override;
    void onStartFight(Entity* user, Enums::Type::Line line) override;
    void use(Entity* user) override;
};