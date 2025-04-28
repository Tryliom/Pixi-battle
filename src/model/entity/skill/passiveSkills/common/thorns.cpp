#include "model/entity/skill/passiveSkills/common/thorns.h"

#include "model/entity/entity.h"

Thorns::Thorns(int level) :
	PassiveSkill("680da09f-d7ab-46c1-80b5-f815c82749b6", "Thorns", Description{
		             .description = "Reflect **skill0%** of the damage received as physical damage.",
		             .parameters = {SkillParameter(Enums::Type::Improvement::Percent, 2)}
	             }, "thorns", Enums::Tier::Common, level, {
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 2},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 2},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 2},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 2},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Special, .description = Description{.description = "The damage are inflicted as true damage"}},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Special, .description = Description{.description = "The damage reflected are calculated before the damage reduction"}},
	             }
	)
{}

void Thorns::onStartDamage(Entity* user, Entity* opponent, float& hp, Enums::Type::Damage damageType, float& damageReductionFlat, float& damageReductionPercent)
{
	if (opponent != nullptr && this->level >= 6)
	{
		reflect(user, opponent, hp);
	}
}

void Thorns::onEndDamage(Entity* user, Entity* opponent, float hp, Enums::Type::Damage damageType)
{
	if (opponent != nullptr && this->level < 6)
	{
		reflect(user, opponent, hp);
	}
}

void Thorns::reflect(Entity* user, Entity* opponent, float hp)
{
	float damagePercentReflected = this->getImprovementTotalValue(Enums::Type::Improvement::Percent);
	float damage = hp * damagePercentReflected;

	if (this->level >= 5)
	{
		opponent->takeDamage(damage, Enums::Type::Damage::True);
	}
	else
	{
		damage = opponent->calculateDamageTaken(damage, Enums::Type::Damage::Physical, 0, 0);
		opponent->takeDamage(damage, Enums::Type::Damage::Physical);
	}
}


