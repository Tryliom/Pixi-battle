#include "model/entity/skill/activeSkills/class/thief/smokeScreen.h"

#include "model/entity/entity.h"
#include "model/entity/fightState.h"
#include "model/fight/participant.h"

SmokeScreen::SmokeScreen(int level) :
	ActiveSkill("bdff8bb5-2e6b-4e70-9c92-e0c8b75d4c00", "Smoke screen", Description{
		            .description = "Grants effect0 for 2 turns and gains **skill0%** of attack bar",
		            .parameters = {SkillParameter(Enums::Type::Improvement::Percent, 15)},
		            .effects = {EffectList::Stealth}
	            },
	            "smokescreen", Enums::Tier::Class, level, {
		            SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 2},
		            SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 3},
		            SkillImprovement{.improvement = Enums::Type::Improvement::Cooldown, .value = 1},
		            SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 10},
		            SkillImprovement{
			            .improvement = Enums::Type::Improvement::Special, .description = Description{
				            .description = "Grants effect0 for 2 turns",
				            .effects = {EffectList::SpeedAugmentation}
			            }}
	            }, 0, 0, 3, Enums::Type::Cost::None, Enums::Type::Damage::None, Enums::Type::Target::Self, Enums::Type::Spell::Status, SkillAnimation("smoke_screen", {}, SkillTargetType::Single))
{}

void SmokeScreen::useStatusSkill(Participant& user, Participant& target)
{
	ActiveSkill::useStatusSkill(user, target);

	//TODO: Add stealth effect

	user.entity->state->attackBarPercent += getImprovementTotalValue(Enums::Type::Improvement::Percent);

	if (this->level >= 5)
	{
		//TODO: Give speed augmentation effect for 2 turns
	}
}

