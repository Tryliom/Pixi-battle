#pragma once

#include <string>
#include <vector>
#include <dpp/nlohmann/json.hpp>

#include "statistics.h"

struct Entity;
struct Player;
struct Monster;
struct ClassType;

struct User
{
	explicit User(std::string id);
	User();

	// Equal to discord ID.
	std::string id;
	Player* player;
	//Inventory inventory;

	// Unlocked classes ids
	std::vector<std::string> unlockedClasses;
	std::vector<Monster*> summons;
	Statistics statistics;

	// Achievements unlocked by the player.
	std::vector<std::string> achievements;

	std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<double>> lastInteraction;

	void updateLastInteraction();
	[[nodiscard]] bool isFreeable() const;

	bool hasAchievement(const std::string& achievementId);
	void unlockAchievement(const std::string& achievementId);

	friend void to_json(nlohmann::json& nlohmann_json_j, const User& nlohmann_json_t);
	friend void from_json(const nlohmann::json& nlohmann_json_j, User& nlohmann_json_t);
};
