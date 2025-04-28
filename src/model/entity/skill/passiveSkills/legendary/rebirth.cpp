#include "model/entity/skill/passiveSkills/legendary/rebirth.h"

#include "model/entity/entity.h"
#include "model/entity/fightState.h"
#include "utility/randomUtility.h"

Rebirth::Rebirth(int level) :
        PassiveSkill("08ffa11a-76c6-428d-bc17-900c8294e00c", "Rebirth", Description{
                .description = "When the user die, revive once with **skill0%** max health.",
                .parameters = {SkillParameter(Enums::Type::Improvement::Percent, 25)}
        }, "rebirth", Enums::Tier::Legendary, level, {
                             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 5},
                             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 10},
                             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 15},
                             SkillImprovement{.improvement = Enums::Type::Improvement::Percent, .value = 20},
                             SkillImprovement{.improvement = Enums::Type::Improvement::Choice, .choices = {
                                     SkillImprovement{.improvement = Enums::Type::Improvement::Special, .description = Description{.description = "Can be revive more than once with a 30% chance"}},
                                     SkillImprovement{.improvement = Enums::Type::Improvement::Special, .description = Description{
                                             .description = "Increase the percent by 25% and gains effect0 for 2 turns after revive",
                                             .effects = {EffectList::ArmorAugmentation}
                                     }},
                                     SkillImprovement{.improvement = Enums::Type::Improvement::Special, .description = Description{
                                             .description = "Decrease the percent by 25% and gains effect0 or effect1 according to your max attack for 2 turns and increase your attack bar of 50% after revive",
                                             .effects = {EffectList::MagicalAttackAugmentation, EffectList::PhysicalAttackAugmentation}
                                     }},
                             }},
                     })
{}

void Rebirth::onDeath(Entity* user, Entity* cause)
{
    if (user->state->isDead())
    {
	    float percent = this->getImprovementTotalValue(Enums::Type::Improvement::Percent);

        if (this->useLeft == 1)
        {
            if (this->improvementsPerLevel[4].choiceIndex == 1)
            {
                percent += 0.25;
            }
            else if (this->improvementsPerLevel[4].choiceIndex == 2)
            {
                percent -= 0.25;
            }

	        float hp = percent * user->state->maxHp;
            user->resurrect(hp);
            this->useLeft--;
        }
        else if (this->improvementsPerLevel[4].choiceIndex == 0 && user->state->checkChance(0.3f))
        {
	        float hp = percent * user->state->maxHp;
            user->resurrect(hp);
        }
    }
}

void Rebirth::onStartFight(Entity* user, Enums::Type::Line line)
{
    this->useLeft = 1;
}