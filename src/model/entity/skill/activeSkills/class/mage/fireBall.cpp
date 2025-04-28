#include "model/entity/skill/activeSkills/class/mage/fireBall.h"

#include "model/entity/entity.h"
#include "model/fight/participant.h"

FireBall::FireBall(int level) :
	ActiveSkill("1f9ed73d-1be9-4d8e-a469-42b5634ba835", "Fire ball", Description{
		            .description = "Throw a fireball who inflicts effect0 on the target for **skill0** turns with **skill1%** chance.",
		            .parameters = {SkillParameter(Enums::Type::Improvement::TurnEffect, 1), SkillParameter(Enums::Type::Improvement::EffectChance, 65)},
		            .effects = {EffectList::Burn}
	            }, "fireball", Enums::Tier::Class, level, {
		            ::SkillImprovement{.improvement = Enums::Type::Improvement::Power, .value = 5},
		            ::SkillImprovement{.improvement = Enums::Type::Improvement::EffectChance, .value = 10},
		            ::SkillImprovement{.improvement = Enums::Type::Improvement::Cooldown, .value = 1},
		            ::SkillImprovement{.improvement = Enums::Type::Improvement::Power, .value = 10},
		            ::SkillImprovement{.improvement = Enums::Type::Improvement::TurnEffect, .value = 1}
	            }, 85, 0, 3, Enums::Type::Cost::None, Enums::Type::Damage::Magical, Enums::Type::Target::SingleEnemy, Enums::Type::Spell::Damage,
	            SkillAnimation("fire_ball", {
		                           AnimationStep{.type = SkillAnimationType::Projectile},
		                           AnimationStep{.type = SkillAnimationType::Impact, .useSkillFrame = 3}
	                           },
	                           SkillTargetType::Single
	            )
	)
{}

void FireBall::useDamageSkill(Participant& user, Participant& target, bool critical, bool dodge)
{
	ActiveSkill::useDamageSkill(user, target, critical, dodge);

	if (dodge) return;

	float chance = this->getImprovementTotalValue(Enums::Type::Improvement::EffectChance);
	int turns = static_cast<int>(this->getImprovementTotalValue(Enums::Type::Improvement::TurnEffect));

	if (user.entity->canApplyEffect(target.entity, chance))
	{
		target.entity->applyEffect(EffectFactory::getBurn(turns, user.entity));
	}
}
