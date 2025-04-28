#pragma once

#include "entity.h"
//#include "ai.h"

struct Player;

struct Monster : public Entity {
	explicit Monster(const std::string& name, ClassType* classType, int level = 1, int experience = 0, float captureChance = 0.f);
	explicit Monster(const Entity& entity);
	Monster();

	// The amount of experience the monster gives when killed.
	int experience{ 0 };
	// The basic chance of capturing the monster.
	float captureChance{ 0 };

	[[nodiscard]] int calculateExperience() const;
	/**
	 * The capture chance is not affected by the level.
	 * Calculate the current capture chance.
	 * @return
	 */
	[[nodiscard]] float calculateCaptureChance(std::unique_ptr<Player> user, float bonusChance, float flatChance) const;

	friend void to_json(nlohmann::json& nlohmann_json_j, const Monster& nlohmann_json_t);
	friend void from_json(const nlohmann::json& nlohmann_json_j, Monster& nlohmann_json_t);
};