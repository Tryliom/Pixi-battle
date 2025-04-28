#pragma once

#include "utility/enums.h"
#include "model/entity/stat.h"

struct Entity;

struct LineBonus
{
	LineBonus(Enums::Type::Line lineType, Stat stat);
	LineBonus() = default;

	Enums::Type::Line line{ Enums::Type::Line::Back };
	Stat stat;
	Stat currentStat;

	void onStartFight(Entity* user, Enums::Type::Line lineType);
	void onEndFight(Entity* user);
};
