#pragma once

#include "model/entity/skill/skill.h"

struct CounterExpert :
        public PassiveSkill
{
    explicit CounterExpert(int level = 0);

    void onSelectTarget(Entity* user, Entity* target, Skill* skill, Target* targetType) override;
};
