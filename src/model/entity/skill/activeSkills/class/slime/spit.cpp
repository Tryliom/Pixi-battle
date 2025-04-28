#include "model/entity/skill/activeSkills/class/slime/spit.h"

#include "model/fight/participant.h"

#include "model/entity/entity.h"
#include "model/entity/fightState.h"

Spit::Spit(int level) :
//TODO: Apply unique debuff for it
	ActiveSkill("70d19736-3e10-4440-a15a-1b35562390b3", "Spit", Description{.description = "Shoot a part of your body at the opponent, inflicting **effect0**."},
	            "spit", Enums::Tier::Class, level, {
		            SkillImprovement{.improvement = Enums::Type::Improvement::Power, .value = 5},
		            SkillImprovement{.improvement = Enums::Type::Improvement::Power, .value = 5},
		            SkillImprovement{.improvement = Enums::Type::Improvement::Power, .value = 5},
		            SkillImprovement{.improvement = Enums::Type::Improvement::Power, .value = 10},
		            SkillImprovement{
			            .improvement = Enums::Type::Improvement::Special, .description = Description{.description = "Has 50% chance to inflict **effect0** for 1 turn on the target", .effects = {EffectList::ArmorDiminution}}}
	            }, 60, 5, 0,
	            Enums::Type::Cost::Health, Enums::Type::Damage::Magical, Enums::Type::Target::SingleEnemy, Enums::Type::Spell::Damage,
	            SkillAnimation("spit", {
		                           AnimationStep{.type = SkillAnimationType::Projectile},
		                           AnimationStep{.type = SkillAnimationType::Impact, .useSkillFrame = 2}
	                           },
	                           SkillTargetType::Single
	            )
	)
{}

void Spit::useDamageSkill(Participant& user, Participant& target, bool critical, bool dodge)
{
	ActiveSkill::useDamageSkill(user, target, critical, dodge);

	if (dodge || this->level != 5) return;

	if (user.entity->canApplyEffect(target.entity, 0.5f))
	{
		target.entity->applyEffect(EffectFactory::getArmorDiminution(1));
	}
}
