#include "model/entity/skill/passiveSkills/class/thief/thiefsLife.h"

#include "model/entity/entity.h"
#include "model/entity/fightState.h"

ThiefsLife::ThiefsLife(int level) :
	PassiveSkill("0c806f13-6bfe-488e-bb3d-b8e02cb3be0f", "Thief's life", Description{
		.description = "The less health you have, the more speed you gains (up to **skill0%**).",
		.parameters = {SkillParameter(Enums::Type::Improvement::Percent, 30)}
	}, "thiefslife", Enums::Tier::Class, level, {
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 5},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 5},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 5},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 5},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Special, .description = Description{.description = "Increase your critical chance up to 15%"}}
	             })
{}

void ThiefsLife::onEndUpdateHP(Entity* user, float hp)
{
	float percent = this->getImprovementTotalValue(Enums::Type::Improvement::Percent);
	this->userStat.speed = (1.f - user->state->getHpPercent()) * percent;

	if (this->level == 5)
	{
		this->userStat.criticalChance = (1.f - user->state->getHpPercent()) * 0.15f;
	}
}

void ThiefsLife::onEndFight(Entity* user)
{
	this->userStat.speed = 0;
	this->userStat.criticalChance = 0;
}