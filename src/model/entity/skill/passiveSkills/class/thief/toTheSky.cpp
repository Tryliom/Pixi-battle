#include "model/entity/skill/passiveSkills/class/thief/toTheSky.h"

#include "model/entity/entity.h"

ToTheSky::ToTheSky(int level) :
	PassiveSkill("d11d890c-07fe-4b27-b0b1-d6e2e6e76d76", "To the sky", Description{
		             .description = "The damage you inflicts are increased by **skill0%** to target that are slower than you.",
		             .parameters = {SkillParameter(Enums::Type::Improvement::Percent, 10)}
	             },
	             "tothesky", Enums::Tier::Class, level, {
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 1},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 2},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 3},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 4},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Special, .description = Description{.description = "Gains 15% physical armor penetration too"}}
	             })
{}

void ToTheSky::onBeforeAttack(Entity* user, Entity* opponent, bool critical, bool dodge)
{
	float percent = this->getImprovementTotalValue(Enums::Type::Improvement::Percent);

	if (user->stats.speed > opponent->stats.speed)
	{
		this->userStat.damageMultiplier = percent;

		if (this->level == 5)
		{
			this->userStat.physicalArmorPenetrationPercent = 0.15f;
		}
	}
}

void ToTheSky::onEndFight(Entity* user)
{
	this->userStat.damageMultiplier = 0;
	this->userStat.physicalArmorPenetrationPercent = 0;
}
