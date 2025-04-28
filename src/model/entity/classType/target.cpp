#include "model/entity/classType/target.h"

#include <utility>

#include "model/entity/entity.h"
#include "model/entity/skill/skill.h"

Target::Target(const std::string& name, Description description, const std::string& emoji, float bonus, float malus)
{
	this->name = name;
	this->description = std::move(description);
	this->emojiName = emoji;
	this->bonus = bonus;
	this->malus = malus;
}

void Target::onSelectTarget(Entity* user, Entity* target, Skill* skill)
{}

void Target::applyEffects(Entity* user, Entity* target, bool critical)
{
	if (critical)
	{
		this->bonusPercent += 1.f;
	}
}

std::string Target::getDescription(Entity* user, Entity* target)
{
	std::string desc = this->description.asString();
	user->addTargetExtraDescriptionAsUser(this, desc);
	target->addTargetExtraDescriptionAsTarget(this, desc);

	return desc;
}

bool Target::canBeUsed(Entity* user, Entity* target)
{
	return true;
}

float Target::getTotalBonus() const
{
	return this->bonus * (1 + this->bonusPercent);
}

float Target::getTotalMalus() const
{
	return this->malus * (1 + this->malusPercent);
}