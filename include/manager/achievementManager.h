#pragma once

#include <vector>
#include <map>
#include <array>
#include <string>

#include "utility/entityEvents.h"

struct Player;

enum class AchievementName {
    BishopOfCounterAttack,
    Lucky,
    END
};

struct Achievement
{
	std::string id{};
    std::string name{};
    std::string howToUnlock{};
    bool hidden{ false };
	bool permanent{ false };
};

struct AchievementManager : public EntityEvents
{
private:
    AchievementManager();

    void unlock(Entity* entity, AchievementName achievementName);

public:
    std::array<Achievement, static_cast<int>(AchievementName::END)> achievements;

    Achievement getAchievement(const std::string& id);
    static std::vector<Achievement> getLockedAchievements(Player* player);

    void onUpdateStatistics(Player* player);
    void onEndDamage(Entity* user, Entity* opponent, float hp, Enums::Type::Damage damageType) override;

    AchievementManager(const AchievementManager&) = delete;
    AchievementManager& operator=(const AchievementManager&) = delete;
    AchievementManager(AchievementManager&&) = delete;
    AchievementManager& operator=(AchievementManager&&) = delete;

    static auto& getInstance()
    {
        static AchievementManager instance;
        return instance;
    }
};