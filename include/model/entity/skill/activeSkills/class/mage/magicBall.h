#pragma once

#include "model/entity/skill/skill.h"

struct MagicBall : public ActiveSkill
{
    explicit MagicBall(int level = 0);

	void useDamageSkill(Participant& user, Participant& target, bool critical, bool dodge) override;
};