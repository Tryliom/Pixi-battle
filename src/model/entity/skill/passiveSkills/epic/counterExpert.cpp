#include "model/entity/skill/passiveSkills/epic/counterExpert.h"

#include "model/entity/classType/targets/head.h"
#include "model/entity/classType/targets/chest.h"
#include "model/entity/classType/targets/legs.h"
#include "model/entity/classType/targets/arms.h"

#include "model/entity/entity.h"
#include "model/entity/fightState.h"

CounterExpert::CounterExpert(int level) :
	PassiveSkill("661dd21d-55c2-4616-95ff-2f9e3d523452", "Counter expert", Description{
		.description = "When you counter attack, reduce the malus inflicted by the choice of the target by **skill0%**.",
		.parameters = {SkillParameter(Enums::Type::Improvement::Percent, 10)}
	}, "counterexpert", Enums::Tier::Epic, level, {
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 5},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 10},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 10},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 15},
		             SkillImprovement{
			             .improvement = Enums::Type::Improvement::Choice, .choices = {
				             {.improvement = Enums::Type::Improvement::Special, .description = Description{.description = "Automatically target the head on a counter attack"}},
				             {.improvement = Enums::Type::Improvement::Special, .description = Description{.description = "Automatically target the arms on a counter attack"}},
				             {.improvement = Enums::Type::Improvement::Special, .description = Description{.description = "Automatically target the legs on a counter attack"}},
				             {.improvement = Enums::Type::Improvement::Special, .description = Description{.description = "Automatically target the chest on a counter attack"}}
			             }}
	             })
{}

void CounterExpert::onSelectTarget(Entity* user, Entity* target, Skill* skill, Target* targetType)
{
	float percent = this->getImprovementTotalValue(Enums::Type::Improvement::Percent);

	if (this->level == 5)
	{
		if (this->improvementsPerLevel[4].choiceIndex == 0)
		{
			targetType = new Head();
		}
		else if (this->improvementsPerLevel[4].choiceIndex == 1)
		{
			targetType = new Arms();
		}
		else if (this->improvementsPerLevel[4].choiceIndex == 2)
		{
			targetType = new Legs();
		}
		else if (this->improvementsPerLevel[4].choiceIndex == 3)
		{
			targetType = new Chest();
		}
	}

	if (user->state->counterAttack)
	{
		targetType->malusPercent -= percent;
	}
}