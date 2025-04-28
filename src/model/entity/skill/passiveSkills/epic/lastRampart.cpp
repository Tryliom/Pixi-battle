#include "model/entity/skill/passiveSkills/epic/lastRampart.h"

#include "model/entity/entity.h"
#include "model/entity/fightState.h"

LastRampart::LastRampart(int level) :
	PassiveSkill("bdc9b8bd-7eeb-4777-9076-f746a9abd8ef", "Last rampart", Description{
		             .description = "When life is less than 10% health after damage, you get effect0 and effect1 equal to **skill0%** of your max health for 2 turns. Activate once per fight.",
		             .parameters = {SkillParameter(Enums::Type::Improvement::Percent, 10)},
		             .effects = {EffectList::ArmorAugmentation, EffectList::Shield}
	             }, "lastrampart", Enums::Tier::Epic, level, {
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 1},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 2},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 3},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 4},
		             SkillImprovement{
			             .improvement = Enums::Type::Improvement::Choice, .choices = {
				             {.improvement = Enums::Type::Improvement::Special, .description = Description{.description = "Gains 20% attack bar"}},
				             {.improvement = Enums::Type::Improvement::Special, .description = Description{.description = "All debuffs are cleared before applying the buffs"}},
			             }},
	             }
	)
{
	this->canUse = true;
}

void LastRampart::onEndDamage(Entity* user, Entity* opponent, float hp, Enums::Type::Damage damageType)
{
	if (this->canUse && user->state->isHpBelow(10))
	{
		this->use(user);
	}
}

void LastRampart::use(Entity* user)
{
	//TODO: Apply buffs
    float shieldValue = getImprovementTotalValue(Enums::Type::Improvement::Percent) * user->state->maxHp;

    user->applyEffect(EffectFactory::getArmorAugmentation(2));
    user->applyEffect(EffectFactory::getShield(2, shieldValue));

	this->canUse = false;
}

void LastRampart::onStartFight(Entity* user, Enums::Type::Line line)
{
    this->canUse = true;
}
