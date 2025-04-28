#pragma once

#include "utility/enums.h"

struct Entity;
struct Effect;
struct Target;
struct Skill;

struct EntityEvents {
	virtual void onStartTurn(Entity* user) {}
	virtual void onEndTurn(Entity* user, Skill* attackUsed) {}
	virtual void onStartFight(Entity* user, Enums::Type::Line line) {}
	virtual void onEndFight(Entity* user) {}
	virtual void onBeforeAttack(Entity* user, Entity* opponent, bool critical, bool dodge) {}
	virtual void onStartAttack(Entity* user, Entity* opponent, bool critical, bool dodge) {}
	virtual void onEndAttack(Entity* user, Entity* opponent, bool critical, bool dodge) {}
	virtual void onConsumeAttack(Entity* user) {}
	virtual void onMissedAttack(Entity* user, Entity* opponent) {}
	virtual void onDodge(Entity* user, Entity* opponent, bool& dodge) {}
	virtual void onCritical(Entity* user, Entity* opponent, bool& critical) {}
	virtual void onStartDamage(Entity* user, Entity* opponent, float& hp, Enums::Type::Damage type, float& damageReductionFlat, float& damageReductionPercent) {}
	virtual void onEndDamage(Entity* user, Entity* opponent, float hp, Enums::Type::Damage type) {}
	virtual void onDeath(Entity* user, Entity* cause) {}
	virtual void onEndUpdateHP(Entity* user, float hp) {}
	virtual void onStartHeal(Entity* user, float& hp) {}
	virtual void onEndHeal(Entity* user, float hp) {}
	virtual void onReceiveEffect(Entity* user, Effect* effect) {}
	virtual void onOpponentReceiveEffect(Entity* user, Entity* opponent, Effect* effect) {}
	virtual void onOpponentDeath(Entity* user, Entity* opponent, Entity* cause) {}
	virtual void onOpponentMissedAttack(Entity* user, Entity* opponent) {}
	virtual void onOpponentCritical(Entity* user, Entity* opponent) {}
	virtual void onReceiveAttack(Entity* user, Entity* opponent, Skill* attack, bool critical, bool dodge, bool counterAttack) {}
	virtual void onSelectTarget(Entity* user, Entity* target, Skill* skill, Target* targetType) {}
	virtual void onBeingSelectAsTarget(Entity* user, Entity* attacker, Skill* skill, Target* targetType) {}
	virtual void onResurrect(Entity* user, float& hp) {}
	virtual void onOpponentStartTurn(Entity* user, Entity* opponent) {}
	virtual void onOpponentEndTurn(Entity* user, Entity* opponent, Skill* attackUsed) {}
	virtual void onAllyStartDamage(Entity* user, Entity* ally, Entity* opponent, float& hp, Enums::Type::Damage type, float& damageReductionFlat, float& damageReductionPercent) {}
};