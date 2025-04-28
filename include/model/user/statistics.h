#pragma once

#include <map>
#include "string"
#include <dpp/nlohmann/json.hpp>

#include "utility/entityEvents.h"

struct Player;
struct Entity;

struct Statistics :
        public EntityEvents
{
    Statistics();

    std::map<std::string, int> monsterKilled;
    int playerKilled;
    int kills;
    std::map<std::string, int> totalMonsterKilled;
    int totalPlayerKilled;
    int totalKills;
    int bestKills;

    int deaths;

    int totalLevels;
    int bestLevel;

    int totalSummons;

    int totalSuccessfulHitInHead;
    int successfulHitInHead;
    int successfulHitInHeadInABattle;

    int totalSuccessfulHitInChest;
    int successfulHitInChest;
    int successfulHitInChestInABattle;

    int totalSuccessfulHitInLegs;
    int successfulHitInLegs;
    int successfulHitInLegsInABattle;

    int totalSuccessfulHitInArms;
    int successfulHitInArms;
    int successfulHitInArmsInABattle;

    int totalSuccessfulHitInBack;
    int successfulHitInBack;
    int successfulHitInBackInABattle;

    int totalCounterAttack;
    int counterAttacks;
    int counterAttacksInABattle;

    int totalTakenHit;
    int takenHit;
    int takenHitInABattle;

    int totalAchievementsUnlocked;

    void onUnlockAchievement(Entity* entity);
    static void onUpdateStatistics(Entity* entity);
    void onLevelUp(Entity* entity);

    void onEndFight(Entity* user) override;
    void onBeforeAttack(Entity* user, Entity* opponent, bool critical, bool dodge) override;
    void onDeath(Entity* user, Entity* cause) override;
    void onOpponentDeath(Entity* user, Entity* opponent, Entity* cause) override;
    void onReceiveAttack(Entity* user, Entity* opponent, Skill* attack, bool critical, bool dodge, bool counterAttack) override;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        Statistics, monsterKilled, playerKilled, kills, totalMonsterKilled, totalPlayerKilled, totalKills, bestKills, deaths, totalLevels, bestLevel, totalSummons,
        totalSuccessfulHitInHead, successfulHitInHead, totalSuccessfulHitInChest, successfulHitInChest, totalSuccessfulHitInLegs, successfulHitInLegs,
        totalSuccessfulHitInArms, successfulHitInArms, totalSuccessfulHitInBack, successfulHitInBack, totalCounterAttack, counterAttacks,
        totalTakenHit, takenHit, totalAchievementsUnlocked
    );
};
