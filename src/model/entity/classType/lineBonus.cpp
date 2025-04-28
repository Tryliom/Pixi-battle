#include "model/entity/classType/lineBonus.h"

#include "model/entity/entity.h"

LineBonus::LineBonus(Enums::Type::Line lineType, Stat stat)
{
	this->line = lineType;
	this->stat = stat;
}

void LineBonus::onStartFight(Entity* user, Enums::Type::Line lineType)
{
	if (lineType == this->line)
	{
		this->currentStat = this->stat;
	}
}

void LineBonus::onEndFight(Entity* user)
{
	this->currentStat = Stat();
}