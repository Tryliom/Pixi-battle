#include "model/user/statistics.h"

#include "manager/achievementManager.h"

#include "model/entity/classType/targets/head.h"
#include "model/entity/classType/targets/chest.h"
#include "model/entity/classType/targets/legs.h"
#include "model/entity/classType/targets/arms.h"
#include "model/entity/classType/targets/back.h"

#include "model/entity/entity.h"
#include "model/entity/player.h"
#include "model/entity/fightState.h"
#include "model/entity/classType/classType.h"

Statistics::Statistics()
{
	this->monsterKilled = {};
	this->totalMonsterKilled = {};
	this->counterAttacksInABattle = 0;
	this->counterAttacksInABattle = 0;
	this->successfulHitInArmsInABattle = 0;
	this->successfulHitInLegsInABattle = 0;
	this->successfulHitInChestInABattle = 0;
	this->successfulHitInHeadInABattle = 0;
	this->successfulHitInBackInABattle = 0;
	this->totalKills = 0;
	this->totalPlayerKilled = 0;
	this->totalLevels = 0;
	this->totalSummons = 0;
	this->totalSuccessfulHitInChest = 0;
	this->totalSuccessfulHitInHead = 0;
	this->totalSuccessfulHitInLegs = 0;
	this->totalSuccessfulHitInArms = 0;
	this->totalSuccessfulHitInBack = 0;
	this->totalTakenHit = 0;
	this->totalAchievementsUnlocked = 0;
	this->totalCounterAttack = 0;
	this->playerKilled = 0;
	this->kills = 0;
	this->deaths = 0;
	this->bestKills = 0;
	this->bestLevel = 0;
	this->successfulHitInChest = 0;
	this->successfulHitInHead = 0;
	this->successfulHitInLegs = 0;
	this->successfulHitInArms = 0;
	this->successfulHitInBack = 0;
	this->takenHit = 0;
	this->counterAttacks = 0;
	this->takenHitInABattle = 0;
}

void Statistics::onUpdateStatistics(Entity* entity)
{
	auto* player = dynamic_cast<Player*>(entity);
    AchievementManager::getInstance().onUpdateStatistics(player);
}

void Statistics::onUnlockAchievement(Entity* entity)
{
	totalAchievementsUnlocked++;

	this->onUpdateStatistics(entity);
}

void Statistics::onLevelUp(Entity* entity)
{
	totalLevels++;

	this->onUpdateStatistics(entity);
}

void Statistics::onDeath(Entity* user, Entity* cause)
{
	deaths++;

	this->onUpdateStatistics(user);
}

void Statistics::onEndFight(Entity* user)
{
	successfulHitInHeadInABattle = 0;
	successfulHitInChestInABattle = 0;
	successfulHitInLegsInABattle = 0;
	successfulHitInArmsInABattle = 0;
	successfulHitInBackInABattle = 0;
	counterAttacksInABattle = 0;
	takenHitInABattle = 0;

	this->onUpdateStatistics(user);
}

void Statistics::onBeforeAttack(Entity* user, Entity* opponent, bool critical, bool dodge)
{
	if (!dodge)
	{
		if (dynamic_cast<Head*>(user->state->currentTarget))
		{
			totalSuccessfulHitInHead++;
			successfulHitInHead++;
			successfulHitInHeadInABattle++;
		}
		if (dynamic_cast<Chest*>(user->state->currentTarget))
		{
			totalSuccessfulHitInChest++;
			successfulHitInChest++;
			successfulHitInChestInABattle++;
		}
		if (dynamic_cast<Legs*>(user->state->currentTarget))
		{
			totalSuccessfulHitInLegs++;
			successfulHitInLegs++;
			successfulHitInLegsInABattle++;
		}
		if (dynamic_cast<Arms*>(user->state->currentTarget))
		{
			totalSuccessfulHitInArms++;
			successfulHitInArms++;
			successfulHitInArmsInABattle++;
		}
		if (dynamic_cast<Back*>(user->state->currentTarget))
		{
			totalSuccessfulHitInBack++;
			successfulHitInBack++;
			successfulHitInBackInABattle++;
		}
	}
	if (user->state->counterAttack)
	{
		totalCounterAttack++;
		counterAttacks++;
		counterAttacksInABattle++;
	}

	this->onUpdateStatistics(user);
}

void Statistics::onReceiveAttack(Entity* user, Entity* opponent, Skill* attack, bool critical, bool dodge, bool counterAttack)
{
	totalTakenHit++;
	takenHit++;
	takenHitInABattle++;

	this->onUpdateStatistics(user);
}

void Statistics::onOpponentDeath(Entity* user, Entity* opponent, Entity* cause)
{
	if (opponent->state->isDead())
	{
		kills++;
		bestKills++;
		totalKills++;

		if (opponent->type == Enums::Type::Entity::Monster)
		{
			if (!monsterKilled[opponent->classType->id])
			{
				monsterKilled[opponent->classType->id] = 0;
			}
			if (!totalMonsterKilled[opponent->classType->id])
			{
				totalMonsterKilled[opponent->classType->id] = 0;
			}
			monsterKilled[opponent->classType->id]++;
			totalMonsterKilled[opponent->classType->id]++;
		}
		else if (opponent->type == Enums::Type::Entity::Player)
		{
			playerKilled++;
			totalPlayerKilled++;
		}
	}

	this->onUpdateStatistics(user);
}