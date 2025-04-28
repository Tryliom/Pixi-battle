#pragma once

#include "model/entity/skill/skill.h"

struct FireBall : public ActiveSkill
{
    explicit FireBall(int level = 0);

	void useDamageSkill(Participant& user, Participant& target, bool critical, bool dodge) override;
};
