#include "model/entity/skill/activeSkills/class/thief/daggerThrow.h"

#include "model/entity/entity.h"
#include "model/entity/fightState.h"
#include "model/fight/participant.h"

DaggerThrow::DaggerThrow(int level) :
	ActiveSkill("e0db91a5-e522-4c41-9258-dcbd4dd76af4", "Dagger Throw", Description{.description = "Throw a dagger at the opponent."},
	            "daggerthrow", Enums::Tier::Class, level, {
		            SkillImprovement{.improvement = Enums::Type::Improvement::Power, .value = 5},
		            SkillImprovement{.improvement = Enums::Type::Improvement::Power, .value = 5},
		            SkillImprovement{.improvement = Enums::Type::Improvement::Power, .value = 5},
		            SkillImprovement{.improvement = Enums::Type::Improvement::Power, .value = 10},
		            SkillImprovement{
			            .improvement = Enums::Type::Improvement::Choice, .choices = {
				            SkillImprovement{.improvement = Enums::Type::Improvement::Special, .description = Description{.description = "Give a dodge buff for 1 turn after the attack"}},
				            SkillImprovement{
					            .improvement = Enums::Type::Improvement::Special, .description = Description{
						            .description = "Has 50% chance to inflict effect0 on the target for 2 turns",
						            .effects = {EffectList::Bleeding}
					            }},
			            }}
	            }, 65, 0, 0,
	            Enums::Type::Cost::None, Enums::Type::Damage::Physical, Enums::Type::Target::SingleEnemy, Enums::Type::Spell::Damage,
	            SkillAnimation("dagger_throw", {
		            AnimationStep{.type = SkillAnimationType::Projectile},
		            AnimationStep{.type = SkillAnimationType::Impact, .useSkillFrame = 1}
				}, SkillTargetType::Single)
	)
{}

void DaggerThrow::useDamageSkill(Participant& user, Participant& target, bool critical, bool dodge)
{
	ActiveSkill::useDamageSkill(user, target, critical, dodge);

	if (dodge || this->level < 5) return;

	if (this->improvementsPerLevel[4].choiceIndex == 0)
	{
		target.entity->applyEffect(EffectFactory::getDodgeChanceAugmentation(1));
	}
	else if (this->improvementsPerLevel[4].choiceIndex == 1)
	{
		if (user.entity->canApplyEffect(target.entity, 0.5f))
		{
			target.entity->applyEffect(EffectFactory::getBleeding(2, user.entity));
		}
	}
}
