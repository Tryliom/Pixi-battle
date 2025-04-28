#include "model/entity/monster.h"

#include "model/entity/fightState.h"
#include "model/entity/player.h"
#include "model/entity/classType/classType.h"

Monster::Monster(const std::string& name, ClassType* classType, int level, int experience, float captureChance) :
	Entity(name, classType, Enums::Type::Entity::Monster)
{
	this->experience = experience;
	this->captureChance = captureChance;

	updateLevel(level);
}

Monster::Monster(const Entity& entity) : Entity(entity)
{
	this->experience = 0;
	this->captureChance = 0;
}

Monster::Monster() : Entity()
{
	this->experience = 0;
	this->captureChance = 0;
}

int Monster::calculateExperience() const
{
	return (int) (experience * (1 + 0.1 * (this->classType->level - 1)));
}

float Monster::calculateCaptureChance(std::unique_ptr<Player> user, float bonusChance, float flatChance) const
{
	float multiplier = 1.f + bonusChance;

	// Level difference
	auto levelMaxDifference = static_cast<float>(user->getLevel()) * 0.1f;
	auto levelDifference = static_cast<float>(user->getLevel() - this->getLevel());

	if (user->classType->level <= 20) {
		levelMaxDifference = 5;
	}

	float levelDifferenceMultiplier = 0.5f + (levelDifference / levelMaxDifference);

	if (levelDifferenceMultiplier < 0.5f)
	{
		levelDifferenceMultiplier = 0.5f;
	}

	if (levelDifference < 0)
	{
		multiplier -= levelDifferenceMultiplier;
	}
	else
	{
		multiplier += levelDifferenceMultiplier;
	}

	if (user->state->active && this->state->active) {
		// Health difference
		multiplier += 0.2f - this->state->getHpPercent();

		// Status difference
		multiplier += 0.1f * static_cast<float>(user->state->countBuffs()) - 0.1f * static_cast<float>(this->state->countBuffs());
		multiplier += 0.1f * static_cast<float>(this->state->countDebuffs()) - 0.1f * static_cast<float>(user->state->countDebuffs());
	}

	if (multiplier < 0.1f)
	{
		multiplier = 0.1f;
	}

	return captureChance * multiplier + flatChance;
}

void to_json(json& nlohmann_json_j, const Monster& nlohmann_json_t)
{
	nlohmann_json_j = static_cast<Entity>(nlohmann_json_t);
}

void from_json(const json& nlohmann_json_j, Monster& nlohmann_json_t)
{
	Entity entity = nlohmann_json_j;
	nlohmann_json_t = static_cast<Monster>(entity);
}

