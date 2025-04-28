#pragma once

#include "model/entity/skill/skill.h"

struct Rebirth :
        PassiveSkill
{
    explicit Rebirth(int level = 0);

    int useLeft{1};

    void onDeath(Entity* user, Entity* cause) override;
    void onStartFight(Entity* user, Enums::Type::Line line) override;
};
