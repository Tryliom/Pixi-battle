#pragma once

#include "model/entity/entity.h"

struct Player : public Entity
{
	explicit Player(const std::string& name, ClassType* classType = nullptr);
	explicit Player(const Entity& entity);
	Player();

	std::string userID;

	void setUser(const std::string& userID);

    void onLevelUp() override;

	// Events
	void onResurrect(float& hp) override;
	void onSelectTarget(Entity* target, Skill* skill, Target* targetType) override;
	void onBeingSelectAsTarget(Entity* attacker, Skill* skill, Target* targetType) override;
	void onReceiveAttack(Entity* attacker, Skill* skill, bool critical, bool dodge, bool counterAttack = false) override;
	void onOpponentStartTurn(Entity* opponent) override;
	void onOpponentEndTurn(Entity* opponent, Skill* attackUsed) override;
	void onStartTurn() override;
	void onEndTurn(Skill* attackUsed) override;
	void onStartFight(const Participants& participants, Enums::Type::Line line, bool resetState = true) override;
	void onEndFight();
	/**
	 * Called just before start calculating the damage. Update stats.
	 * @param opponent
	 * @param critical
	 * @param dodge
	 */
	void onBeforeAttack(Entity* opponent, bool critical, bool dodge) override;
	void onStartAttack(Entity* opponent, bool critical, bool dodge) override;
	void onEndAttack(Entity* opponent, bool critical, bool dodge) override;
	void onConsumeAttack() override;
	void onMissedAttack(Entity* opponent) override;
	void onDodge(Entity* opponent, bool& dodge) override;
	void onCritical(Entity* opponent, bool& critical) override;
	void onStartDamage(Entity* opponent, float& hp, Enums::Type::Damage damageType, float& damageReductionFlat, float& damageReductionPercent) override;
	void onEndDamage(Entity* opponent, float hp, Enums::Type::Damage damageType) override;
	void onDeath(Entity* cause) override;
	void onEndUpdateHP(float hp) override;
	void onStartHeal(float& hp) override;
	void onEndHeal(float hp) override;
	void onReceiveEffect(Effect* effect) override;
	void onOpponentReceiveEffect(Entity* opponent, Effect* effect) override;
	void onOpponentDeath(Entity* opponent, Entity* cause) override;
	void onOpponentMissedAttack(Entity* opponent) override;
	void onOpponentCritical(Entity* opponent) override;
	void onAllyStartDamage(Entity* ally, Entity* opponent, float& hp, Enums::Type::Damage damageType, float& damageReductionFlat, float& damageReductionPercent) override;

	friend void to_json(nlohmann::json& nlohmann_json_j, const Player& nlohmann_json_t);
	friend void from_json(const nlohmann::json& nlohmann_json_j, Player& nlohmann_json_t);
};