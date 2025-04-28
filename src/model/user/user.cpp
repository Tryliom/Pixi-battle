#include "model/user/user.h"

#include "manager/classManager.h"

#include "model/entity/entity.h"
#include "model/entity/player.h"
#include "model/entity/monster.h"
#include "model/user/statistics.h"
#include "model/entity/classType/classType.h"

#include <utility>

User::User(std::string id)
{
	this->id = std::move(id);
	this->player = nullptr;
	this->statistics = Statistics();
	this->summons = {};
	this->unlockedClasses = {Mage().id, Thief().id};
	this->lastInteraction = std::chrono::system_clock::now();
}

User::User()
{
	this->id = "";
	this->player = nullptr;
	this->statistics = Statistics();
	this->summons = {};
	this->unlockedClasses = {Mage().id, Thief().id};
	this->lastInteraction = std::chrono::system_clock::now();
}

void to_json(nlohmann::json& nlohmann_json_j, const User& nlohmann_json_t)
{
	nlohmann_json_j["id"] = nlohmann_json_t.id;
	if (nlohmann_json_t.player != nullptr)
	{
		nlohmann_json_j["player"] = *nlohmann_json_t.player;
	}
	nlohmann_json_j["achievements"] = nlohmann_json_t.achievements;
	nlohmann_json_j["statistics"] = nlohmann_json_t.statistics;
	json summons = {}, unlockedClasses = {};

	for (auto summon: nlohmann_json_t.summons)
	{
		summons.emplace_back(*summon);
	}

	for (const auto& unlockedClass: nlohmann_json_t.unlockedClasses)
	{
		unlockedClasses.emplace_back(unlockedClass);
	}

	if (!summons.empty())
	{
		nlohmann_json_j["summons"] = summons;
	}

	if (!unlockedClasses.empty())
	{
		nlohmann_json_j["unlockedClasses"] = unlockedClasses;
	}
}

void from_json(const json& nlohmann_json_j, User& nlohmann_json_t)
{
	nlohmann_json_j.at("id").get_to(nlohmann_json_t.id);
	if (nlohmann_json_j.find("player") != nlohmann_json_j.end())
	{
		Player entity = nlohmann_json_j.at("player");
		nlohmann_json_t.player = new Player(entity);
	}

	nlohmann_json_j.at("statistics").get_to(nlohmann_json_t.statistics);

	if (nlohmann_json_j.contains("achievements"))
	{
		for (std::string achievement: nlohmann_json_j["achievements"])
		{
			nlohmann_json_t.achievements.emplace_back(achievement);
		}
	}

	if (nlohmann_json_j.find("summons") != nlohmann_json_j.end())
	{
		for (Monster summon: nlohmann_json_j["summons"])
		{
			nlohmann_json_t.summons.emplace_back(new Monster(summon));
		}
	}
	if (nlohmann_json_j.find("unlockedClasses") != nlohmann_json_j.end())
	{
		for (std::string id: nlohmann_json_j["unlockedClasses"])
		{
			bool found = false;

			for (const std::string& classId: nlohmann_json_t.unlockedClasses)
			{
				if (classId == id)
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				nlohmann_json_t.unlockedClasses.emplace_back(id);
			}
		}
	}
}

void User::updateLastInteraction()
{
	this->lastInteraction = std::chrono::system_clock::now();
}

bool User::isFreeable() const
{
	return std::chrono::system_clock::now() - this->lastInteraction > std::chrono::minutes(5) && (this->player == nullptr || this->player->isFreeable());
}

bool User::hasAchievement(const std::string& achievementId)
{
	for (const std::string& achievement: this->achievements)
	{
		if (achievement == achievementId)
		{
			return true;
		}
	}

	return false;
}

void User::unlockAchievement(const std::string& achievementId)
{
	this->achievements.emplace_back(achievementId);
}