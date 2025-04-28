#include "model/entity/skill/passiveSkills/class/mage/illumination.h"

Illumination::Illumination(int level) :
	PassiveSkill("1fc25258-aa24-401f-8d1b-5634246698fc", "Illumination", Description{
		             .description = "Gains **skill0 stack** every time you use a skill up to a maximum of **skill1**. Each stack increases your magic attack by 1%.",
		             .parameters = {SkillParameter(Enums::Type::Improvement::Stack, 1), SkillParameter(Enums::Type::Improvement::MaxStack, 10)}
	             },
	             "illumination", Enums::Tier::Class, level, {
		             SkillImprovement{.improvement = Enums::Type::Improvement::MaxStack, .value = 5},
		             SkillImprovement{.improvement = Enums::Type::Improvement::MaxStack, .value = 5},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Stack, .value = 1},
		             SkillImprovement{.improvement = Enums::Type::Improvement::MaxStack, .value = 10},
		             SkillImprovement{.improvement = Enums::Type::Improvement::Special, .description = Description{.description = "Each stacks increases your effectiveness by 1% too"}}
	             })
{
	this->currentStack = 0;
}

void Illumination::onConsumeAttack(Entity* user)
{
	int maxStack = this->getImprovementTotalValueForDisplay(Enums::Type::Improvement::MaxStack);
	int stack = this->getImprovementTotalValueForDisplay(Enums::Type::Improvement::Stack);
	this->currentStack += stack;

	if (this->currentStack > maxStack)
	{
		this->currentStack = maxStack;
	}

	this->updateStacks();
}

void Illumination::updateStacks()
{
	this->userStat.magicalAttackPercent = static_cast<float>(this->currentStack) / 100.f;

	if (this->level == 5)
	{
		this->userStat.accuracy = static_cast<float>(this->currentStack) / 100.f;
	}
}

void Illumination::onEndFight(Entity* user)
{
	this->currentStack = 0;
	this->updateStacks();
}