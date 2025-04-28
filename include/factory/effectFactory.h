#pragma once

#include "model/entity/effect/effect.h"

enum class EffectList {
	AccuracyDiminution,
	ArmorDiminution,
	AttackDiminution,
	CriticalChanceDiminution,
	CriticalDamageDiminution,
	DodgeChanceDiminution,
	MagicalArmorDiminution,
	MagicalAttackDiminution,
	PhysicalArmorDiminution,
	PhysicalAttackDiminution,
	SpeedDiminution,
	AccuracyAugmentation,
	ArmorAugmentation,
	AttackAugmentation,
	CriticalChanceAugmentation,
	CriticalDamageAugmentation,
	DodgeChanceAugmentation,
	MagicalArmorAugmentation,
	MagicalAttackAugmentation,
	PhysicalArmorAugmentation,
	PhysicalAttackAugmentation,
	SpeedAugmentation,
	Stealth,
	Poison,
	Bleeding,
	Burn,
	Shield
};

struct EffectData {
	std::string name;
	std::string emojiName;
	std::string description;
};

namespace EffectFactory {
	EffectData getEffectData(EffectList effect);
	Effect* getAccuracyDiminution(int turns);
	Effect* getArmorDiminution(int turns);
	Effect* getAttackDiminution(int turns);
	Effect* getCriticalChanceDiminution(int turns);
	Effect* getCriticalDamageDiminution(int turns);
	Effect* getDodgeChanceDiminution(int turns);
	Effect* getMagicalArmorDiminution(int turns);
	Effect* getMagicalAttackDiminution(int turns);
	Effect* getPhysicalArmorDiminution(int turns);
	Effect* getPhysicalAttackDiminution(int turns);
	Effect* getSpeedDiminution(int turns);
	Effect* getAccuracyAugmentation(int turns);
	Effect* getArmorAugmentation(int turns);
	Effect* getAttackAugmentation(int turns);
	Effect* getCriticalChanceAugmentation(int turns);
	Effect* getCriticalDamageAugmentation(int turns);
	Effect* getDodgeChanceAugmentation(int turns);
	Effect* getMagicalArmorAugmentation(int turns);
	Effect* getMagicalAttackAugmentation(int turns);
	Effect* getPhysicalArmorAugmentation(int turns);
	Effect* getPhysicalAttackAugmentation(int turns);
	Effect* getSpeedAugmentation(int turns);
	Effect* getStealth(int turns);
	Effect* getPoison(int turns);
	Effect* getBleeding(int turns, Entity* attacker);
	Effect* getBurn(int turns, Entity* attacker);
	Effect* getShield(int turns, float amount);
};