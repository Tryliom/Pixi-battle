#include "model/entity/skill/passiveSkills/class/mage/curse.h"

#include "model/entity/entity.h"
#include "model/entity/fightState.h"

Curse::Curse(int level) :
	PassiveSkill("cbaf6d12-4221-41bd-ac80-d0613354e1cf", "Curse", Description{
		.description = "Increase your damage by **skill0%** against enemies with debuff.",
		.parameters = {SkillParameter(Enums::Type::Improvement::Percent, 10)}
	}, "curse", Enums::Tier::Class, level, {
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 1},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 1},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 1},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 2},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Special, .description = Description{.description = "**+3%** more damage per debuffs"}}
	             })
{}

void Curse::onBeforeAttack(Entity* user, Entity* opponent, bool critical, bool dodge)
{
	float percent = this->getImprovementTotalValue(Enums::Type::Improvement::Percent);
	int debuffs = 0;

	for (Effect* effect: opponent->state->effects)
	{
		if (effect->type == EffectType::Debuff)
		{
			debuffs++;
		}
	}

	if (debuffs > 0)
	{
		if (this->level == 5)
		{
			percent += 0.03f * static_cast<float>(debuffs);
		}

		this->userStat.damageMultiplier = percent;
	}
}

void Curse::onEndAttack(Entity* user, Entity* opponent, bool critical, bool dodge)
{
	this->userStat.damageMultiplier = 0;
}