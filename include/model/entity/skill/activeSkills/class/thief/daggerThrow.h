#pragma once

#include "model/entity/skill/skill.h"

struct DaggerThrow :
        public ActiveSkill
{
    explicit DaggerThrow(int level = 0);

	void useDamageSkill(Participant& user, Participant& target, bool critical, bool dodge) override;
};
