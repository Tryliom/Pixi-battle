#include "model/entity/player.h"

#include "manager/userManager.h"
#include "manager/skillManager.h"

#include "model/user/user.h"
#include "model/entity/classType/classType.h"
#include "model/entity/fightState.h"
#include "model/entity/skill/skill.h"
#include "manager/achievementManager.h"

Player::Player(const std::string& name, ClassType* classType) : Entity(name, classType, Enums::Type::Entity::Player)
{
	this->userID = "";
}

Player::Player(const Entity& entity) : Entity(entity)
{
	this->userID = "";
}

Player::Player() : Entity()
{
	this->userID = "";
}

void Player::setUser(const std::string& userId)
{
	this->userID = userId;
}

void Player::onLevelUp()
{
    Entity::onLevelUp();

    UserManager::getInstance().getUser(this->userID).statistics.onLevelUp(this);
}

// Events

void Player::onResurrect(float& hp)
{
	Entity::onResurrect(hp);

	UserManager::getInstance().getUser(this->userID).statistics.onResurrect(this, hp);
	AchievementManager::getInstance().onResurrect(this, hp);
}

void Player::onStartTurn()
{
	Entity::onStartTurn();

	UserManager::getInstance().getUser(this->userID).statistics.onStartTurn(this);
	AchievementManager::getInstance().onStartTurn(this);
}

void Player::onEndTurn(Skill* attackUsed)
{
	Entity::onEndTurn(attackUsed);

	UserManager::getInstance().getUser(this->userID).statistics.onEndTurn(this, attackUsed);
	AchievementManager::getInstance().onEndTurn(this, attackUsed);
}

void Player::onSelectTarget(Entity* target, Skill* skill, Target* targetType)
{
	Entity::onSelectTarget(target, skill, targetType);

	UserManager::getInstance().getUser(this->userID).statistics.onSelectTarget(this, target, skill, targetType);
	AchievementManager::getInstance().onSelectTarget(this, target, skill, targetType);
}

void Player::onBeingSelectAsTarget(Entity* attacker, Skill* skill, Target* targetType)
{
	Entity::onBeingSelectAsTarget(attacker, skill, targetType);

	UserManager::getInstance().getUser(this->userID).statistics.onBeingSelectAsTarget(this, attacker, skill, targetType);
	AchievementManager::getInstance().onBeingSelectAsTarget(this, attacker, skill, targetType);
}

void Player::onReceiveAttack(Entity* attacker, Skill* skill, bool critical, bool dodge, bool counterAttack)
{
	Entity::onReceiveAttack(attacker, skill, critical, dodge, counterAttack);

	UserManager::getInstance().getUser(this->userID).statistics.onReceiveAttack(this, attacker, skill, critical, dodge, counterAttack);
	AchievementManager::getInstance().onReceiveAttack(this, attacker, skill, critical, dodge, counterAttack);
}

void Player::onOpponentCritical(Entity* opponent)
{
	Entity::onOpponentCritical(opponent);

	UserManager::getInstance().getUser(this->userID).statistics.onOpponentCritical(this, opponent);
	AchievementManager::getInstance().onOpponentCritical(this, opponent);
}

void Player::onOpponentMissedAttack(Entity* opponent)
{
	Entity::onOpponentMissedAttack(opponent);

	UserManager::getInstance().getUser(this->userID).statistics.onOpponentMissedAttack(this, opponent);
	AchievementManager::getInstance().onOpponentMissedAttack(this, opponent);
}

void Player::onOpponentDeath(Entity* opponent, Entity* cause)
{
	Entity::onOpponentDeath(opponent, cause);

	UserManager::getInstance().getUser(this->userID).statistics.onOpponentDeath(this, opponent, cause);
	AchievementManager::getInstance().onOpponentDeath(this, opponent, cause);
}

void Player::onOpponentReceiveEffect(Entity* opponent, Effect* effect)
{
	Entity::onOpponentReceiveEffect(opponent, effect);

	UserManager::getInstance().getUser(this->userID).statistics.onOpponentReceiveEffect(this, opponent, effect);
	AchievementManager::getInstance().onOpponentReceiveEffect(this, opponent, effect);
}

void Player::onReceiveEffect(Effect* effect)
{
	Entity::onReceiveEffect(effect);

	UserManager::getInstance().getUser(this->userID).statistics.onReceiveEffect(this, effect);
	AchievementManager::getInstance().onReceiveEffect(this, effect);
}

void Player::onStartFight(const Participants& participants, Enums::Type::Line line, bool resetState)
{
	Entity::onStartFight(participants, line, resetState);

	UserManager::getInstance().getUser(this->userID).statistics.onStartFight(this, line);
	AchievementManager::getInstance().onStartFight(this, line);
}

void Player::onEndFight()
{
	Entity::onEndFight();

	UserManager::getInstance().getUser(this->userID).statistics.onEndFight(this);
	AchievementManager::getInstance().onEndFight(this);
}

void Player::onOpponentStartTurn(Entity* opponent)
{
	Entity::onOpponentStartTurn(opponent);

	UserManager::getInstance().getUser(this->userID).statistics.onOpponentStartTurn(this, opponent);
	AchievementManager::getInstance().onOpponentStartTurn(this, opponent);
}

void Player::onOpponentEndTurn(Entity* opponent, Skill* attackUsed)
{
	Entity::onOpponentEndTurn(opponent, attackUsed);

	UserManager::getInstance().getUser(this->userID).statistics.onOpponentEndTurn(this, opponent, attackUsed);
	AchievementManager::getInstance().onOpponentEndTurn(this, opponent, attackUsed);
}

void Player::onStartAttack(Entity* opponent, bool critical, bool dodge)
{
	Entity::onStartAttack(opponent, critical, dodge);

	UserManager::getInstance().getUser(this->userID).statistics.onStartAttack(this, opponent, critical, dodge);
	AchievementManager::getInstance().onStartAttack(this, opponent, critical, dodge);
}

void Player::onEndAttack(Entity* opponent, bool critical, bool dodge)
{
	Entity::onEndAttack(opponent, critical, dodge);

	UserManager::getInstance().getUser(this->userID).statistics.onEndAttack(this, opponent, critical, dodge);
	AchievementManager::getInstance().onEndAttack(this, opponent, critical, dodge);
}

void Player::onConsumeAttack()
{
	Entity::onConsumeAttack();

	UserManager::getInstance().getUser(this->userID).statistics.onConsumeAttack(this);
	AchievementManager::getInstance().onConsumeAttack(this);
}

void Player::onMissedAttack(Entity* opponent)
{
	Entity::onMissedAttack(opponent);

	UserManager::getInstance().getUser(this->userID).statistics.onMissedAttack(this, opponent);
	AchievementManager::getInstance().onMissedAttack(this, opponent);
}

void Player::onCritical(Entity* opponent, bool& critical)
{
	Entity::onCritical(opponent, critical);

	UserManager::getInstance().getUser(this->userID).statistics.onCritical(this, opponent, critical);
	AchievementManager::getInstance().onCritical(this, opponent, critical);
}

void Player::onDodge(Entity* opponent, bool& dodge)
{
	Entity::onDodge(opponent, dodge);

	UserManager::getInstance().getUser(this->userID).statistics.onDodge(this, opponent, dodge);
	AchievementManager::getInstance().onDodge(this, opponent, dodge);
}

void Player::onStartDamage(Entity* opponent, float& hp, Enums::Type::Damage damageType, float& damageReductionFlat, float& damageReductionPercent)
{
	Entity::onStartDamage(opponent, hp, damageType, damageReductionFlat, damageReductionPercent);

	UserManager::getInstance().getUser(this->userID).statistics.onStartDamage(this, opponent, hp, damageType, damageReductionFlat, damageReductionPercent);
	AchievementManager::getInstance().onStartDamage(this, opponent, hp, damageType, damageReductionFlat, damageReductionPercent);
}

void Player::onEndDamage(Entity* opponent, float hp, Enums::Type::Damage damageType)
{
	Entity::onEndDamage(opponent, hp, damageType);

	UserManager::getInstance().getUser(this->userID).statistics.onEndDamage(this, opponent, hp, damageType);
	AchievementManager::getInstance().onEndDamage(this, opponent, hp, damageType);
}

void Player::onDeath(Entity* cause)
{
	Entity::onDeath(cause);

	UserManager::getInstance().getUser(this->userID).statistics.onDeath(this, cause);
	AchievementManager::getInstance().onDeath(this, cause);
}

void Player::onEndUpdateHP(float hp)
{
	Entity::onEndUpdateHP(hp);

	UserManager::getInstance().getUser(this->userID).statistics.onEndUpdateHP(this, hp);
	AchievementManager::getInstance().onEndUpdateHP(this, hp);
}

void Player::onStartHeal(float& hp)
{
	Entity::onStartHeal(hp);

	UserManager::getInstance().getUser(this->userID).statistics.onStartHeal(this, hp);
	AchievementManager::getInstance().onStartHeal(this, hp);
}

void Player::onEndHeal(float hp)
{
	Entity::onEndHeal(hp);

	UserManager::getInstance().getUser(this->userID).statistics.onEndHeal(this, hp);
	AchievementManager::getInstance().onEndHeal(this, hp);
}

void Player::onAllyStartDamage(Entity* ally, Entity* opponent, float& hp, Enums::Type::Damage damageType, float& damageReductionFlat, float& damageReductionPercent)
{
	Entity::onAllyStartDamage(ally, opponent, hp, damageType, damageReductionFlat, damageReductionPercent);

	UserManager::getInstance().getUser(this->userID).statistics.onAllyStartDamage(this, ally, opponent, hp, damageType, damageReductionFlat, damageReductionPercent);
	AchievementManager::getInstance().onAllyStartDamage(this, ally, opponent, hp, damageType, damageReductionFlat, damageReductionPercent);
}

void Player::onBeforeAttack(Entity* opponent, bool critical, bool dodge)
{
	Entity::onBeforeAttack(opponent, critical, dodge);

	UserManager::getInstance().getUser(this->userID).statistics.onBeforeAttack(this, opponent, critical, dodge);
	AchievementManager::getInstance().onBeforeAttack(this, opponent, critical, dodge);
}

void to_json(json& nlohmann_json_j, const Player& nlohmann_json_t)
{
	nlohmann_json_j = static_cast<Entity>(nlohmann_json_t);

	nlohmann_json_j["userID"] = nlohmann_json_t.userID;
}

void from_json(const json& nlohmann_json_j, Player& nlohmann_json_t)
{
	Entity entity = nlohmann_json_j;
	nlohmann_json_t = static_cast<Player>(entity);

	nlohmann_json_t.userID = nlohmann_json_j["userID"];
	nlohmann_json_t.type = Enums::Type::Entity::Player;
}