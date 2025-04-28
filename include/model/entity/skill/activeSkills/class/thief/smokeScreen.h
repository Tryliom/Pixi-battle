#pragma once

#include "model/entity/skill/skill.h"

struct SmokeScreen :
        public ActiveSkill
{
    explicit SmokeScreen(int level = 0);

	void useStatusSkill(Participant& user, Participant& target) override;
};
