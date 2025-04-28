#include "manager/achievementManager.h"

#include "model/entity/entity.h"
#include "model/entity/player.h"
#include "model/entity/fightState.h"

#include "model/user/user.h"

#include "model/entity/skill/passiveSkills/epic/counterExpert.h"
#include "model/entity/skill/passiveSkills/epic/lastRampart.h"

#include "manager/userManager.h"

AchievementManager::AchievementManager()
{
    achievements[static_cast<int>(AchievementName::Lucky)] = Achievement{
	    "c6c81827-3e10-4ea4-9308-e524cf7abcff", "Just lucky enough", "Unlocked by having 10 health or less after damage", true
    };
    achievements[static_cast<int>(AchievementName::BishopOfCounterAttack)] = Achievement{
	    "bbb64cba-dc6c-44b9-bc42-748a32ff4003", "Bishop of the counter attack", "Unlocked by counter attack 10 times in a single fight", false
    };
}

void AchievementManager::unlock(Entity* entity, AchievementName achievementName)
{
	auto* player = dynamic_cast<Player*>(entity);
	auto& user = UserManager::getInstance().getUser(player->userID);

    if (user.hasAchievement(achievements[static_cast<int>(achievementName)].id))
    {
        return;
    }

    if (achievementName == AchievementName::Lucky)
    {
        player->acquireSkill(SkillName::LastRampart, true, false);
    }
    else if (achievementName == AchievementName::BishopOfCounterAttack)
    {
        player->acquireSkill(SkillName::CounterExpert);
    }

	user.unlockAchievement(achievements[static_cast<int>(achievementName)].id);
}

Achievement AchievementManager::getAchievement(const std::string& id)
{
    for (auto& achievement: achievements)
    {
        if (achievement.id == id)
        {
            return achievement;
        }
    }

    return achievements[0];
}

std::vector<Achievement> AchievementManager::getLockedAchievements(Player* player)
{
    std::vector<Achievement> achievements = {};
	auto& user = UserManager::getInstance().getUser(player->userID);

    for (const Achievement& achievement: achievements)
    {
        if (user.hasAchievement(achievement.id))
		{
			continue;
		}

	    achievements.emplace_back(achievement);
    }

    return achievements;
}

void AchievementManager::onUpdateStatistics(Player* player)
{
	Statistics& statistics = UserManager::getInstance().getUser(player->userID).statistics;

    if (statistics.counterAttacksInABattle >= 10)
    {
        unlock(player, AchievementName::BishopOfCounterAttack);
    }
}

void AchievementManager::onEndDamage(Entity* user, Entity* opponent, float hp, Enums::Type::Damage damageType)
{
    if (user->state->hp <= 10.f)
    {
        unlock(dynamic_cast<Player*>(user), AchievementName::Lucky);
    }
}
