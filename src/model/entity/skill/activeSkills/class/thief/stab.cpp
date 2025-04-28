#include "model/entity/skill/activeSkills/class/thief/stab.h"

#include "model/entity/entity.h"
#include "model/entity/fightState.h"
#include "model/fight/participant.h"

Stab::Stab(int level) :
	ActiveSkill("00e57f90-1a44-4f1d-b558-11f191c5a2cc", "Stab", Description{
		            .description = "Stab the enemy and inflicts effect0 for 1 turn with **skill0%** chance.",
		            .parameters = {SkillParameter(Enums::Type::Improvement::Percent, 60)},
		            .effects = {EffectList::Bleeding}
	            },
	            "stab", Enums::Tier::Class, level, {
		            SkillImprovement{.improvement = Enums::Type::Improvement::Power, .value = 5},
		            SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 10},
		            SkillImprovement{.improvement = Enums::Type::Improvement::Cooldown, .value = 1},
		            SkillImprovement{.improvement = Enums::Type::Improvement::Power, .value = 10},
		            SkillImprovement{
			            .improvement = Enums::Type::Improvement::Special, .description = Description{
				            .description = "On a critical hit, inflicts 2 effect0 instead of 1",
				            .effects = {EffectList::Bleeding}
			            }},
	            }, 75, 0, 4, Enums::Type::Cost::None, Enums::Type::Damage::Physical, Enums::Type::Target::SingleEnemy, Enums::Type::Spell::Damage, SkillAnimation("stab", {}, SkillTargetType::Single))
{}

void Stab::useDamageSkill(Participant& user, Participant& target, bool critical, bool dodge)
{
	ActiveSkill::useDamageSkill(user, target, critical, dodge);

	if (dodge) return;

	float chance = getImprovementTotalValue(Enums::Type::Improvement::Percent);

	if (user.entity->canApplyEffect(target.entity, chance))
	{
		target.entity->applyEffect(EffectFactory::getBleeding(1, user.entity));

		if (critical && this->level >= 5)
		{
			target.entity->applyEffect(EffectFactory::getBleeding(1, user.entity));
		}
	}
}
