#pragma once

#include "model/entity/effect/effect.h"

struct Poison : public Effect
{
	explicit Poison(int turns = 0);

	void onStartTurn(Entity* entity) override;
};
