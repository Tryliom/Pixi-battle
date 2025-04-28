#include "model/entity/skill/activeSkills/class/mage/magicBall.h"

#include "model/entity/entity.h"
#include "model/entity/fightState.h"
#include "model/fight/participant.h"

MagicBall::MagicBall(int level) :
	ActiveSkill("473a6f4c-0a00-4b52-bf98-a6ce3d3bde48", "Magic ball", Description{.description = "Send flying a magic ball toward the opponent."},
	            "magicball", Enums::Tier::Class, level, {
		            SkillImprovement{.improvement = Enums::Type::Improvement::Power, .value = 5},
		            SkillImprovement{.improvement = Enums::Type::Improvement::Power, .value = 5},
		            SkillImprovement{.improvement = Enums::Type::Improvement::Power, .value = 5},
		            SkillImprovement{.improvement = Enums::Type::Improvement::Power, .value = 10},
		            SkillImprovement{
			            .improvement = Enums::Type::Improvement::Choice, .choices = {
				            SkillImprovement{.improvement = Enums::Type::Improvement::Special, .description = Description{.description = "Heals the ally or yourself with the lowest HP for 10% of max HP"}},
				            SkillImprovement{
					            .improvement = Enums::Type::Improvement::Special, .description = Description{
						            .description = "Has 50% chance to inflict effect0 on the target for 2 turn",
						            .effects = {EffectList::SpeedDiminution}
					            }},
			            }}
	            }, 60, 0, 0,
	            Enums::Type::Cost::None, Enums::Type::Damage::Magical, Enums::Type::Target::SingleEnemy, Enums::Type::Spell::Damage,
	            SkillAnimation("magic_ball", {
		                           AnimationStep{.type = SkillAnimationType::Projectile},
		                           AnimationStep{.type = SkillAnimationType::Impact, .useSkillFrame = 11}
	                           },
	                           SkillTargetType::Single
	            )
	)
{}

void MagicBall::useDamageSkill(Participant& user, Participant& target, bool critical, bool dodge)
{
	ActiveSkill::useDamageSkill(user, target, critical, dodge);

	if (dodge || this->level < 5) return;

	if (this->improvementsPerLevel[4].choiceIndex == 0)
	{
		Entity* lowestHpAlly = user.entity;

		for (auto* ally : user.entity->state->allies)
		{
			if (ally->state->getHpPercent() < lowestHpAlly->state->getHpPercent())
			{
				lowestHpAlly = ally;
			}
		}

		float healAmount = lowestHpAlly->state->maxHp * 0.1f;

		lowestHpAlly->heal(healAmount, user.entity);
	}
	else if (this->improvementsPerLevel[4].choiceIndex == 1)
	{
		if (user.entity->canApplyEffect(target.entity, 0.5f))
		{
			target.entity->applyEffect(EffectFactory::getSpeedDiminution(2));
		}
	}
}