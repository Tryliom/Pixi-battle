#pragma once

#include <string>

#include "utility/description.h"

struct Entity;
struct Skill;

struct Target
{
	Target(const std::string& name, Description description, const std::string& emoji, float bonus, float malus);

	std::string name;
	// Need to add a point at the end
	Description description;
	std::string emojiName;

	// Apply to the attacker (Like a chance to stun, more damage)
	float bonus;
	float bonusPercent{0.f};
	// Apply to the attacker (Like a chance to miss, less damage)
	float malus;
	float malusPercent{0.f};

	Stat stat;

	// If critical, double the effects and apply them to stats. Happen before attack
	virtual void applyEffects(Entity* user, Entity* target, bool critical);
	// Call to apply malus before calculating dodge chance and critical chance
	virtual void onSelectTarget(Entity* user, Entity* target, Skill* skill);

	std::string getDescription(Entity* user, Entity* target);
	virtual bool canBeUsed(Entity* user, Entity* target);

	//TODO: Add onUse function to do somethings after the attack.

	[[nodiscard]] float getTotalBonus() const;
	[[nodiscard]] float getTotalMalus() const;
};