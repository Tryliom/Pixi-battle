#pragma once

#include "model/entity/skill/skill.h"

struct Thorns :
        public PassiveSkill
{
    explicit Thorns(int level = 0);

	void onStartDamage(Entity* user, Entity* opponent, float &hp, Enums::Type::Damage damageType, float& damageReductionFlat, float& damageReductionPercent) override;
    void onEndDamage(Entity* user, Entity* opponent, float hp, Enums::Type::Damage damageType) override;

	void reflect(Entity* user, Entity* opponent, float hp);
};
