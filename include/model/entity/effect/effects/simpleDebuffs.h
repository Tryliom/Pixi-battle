#pragma once

#include "model/entity/effect/effect.h"

struct AccuracyDiminution :
	public Effect
{
	explicit AccuracyDiminution(int turns = 0) :
		Effect("Accuracy diminution", "accuracyDiminution", "-50% of accuracy",
		       EffectType::Debuff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.accuracy = -0.5}
		)
	{}
};

struct ArmorDiminution :
	public Effect
{
	explicit ArmorDiminution(int turns = 0) :
		Effect("Armor diminution", "armorDiminution", "-40% of final physical and magic armor",
		       EffectType::Debuff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.finalPhysicalArmorPercent = -0.4, .finalMagicalArmorPercent = -0.4}
		)
	{}
};

struct AttackDiminution :
	public Effect
{
	explicit AttackDiminution(int turns = 0) :
		Effect("Attack diminution", "attackDiminution", "-40% of final physical and magic attack",
		       EffectType::Debuff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.finalPhysicalAttackPercent = -0.4, .finalMagicalAttackPercent = -0.4}
		)
	{}
};

struct CriticalChanceDiminution :
	public Effect
{
	explicit CriticalChanceDiminution(int turns = 0) :
		Effect("Critical chance diminution", "criticalChanceDiminution", "-50% of critical chance",
		       EffectType::Debuff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.criticalChance = -0.5}
		)
	{}
};

struct CriticalDamageDiminution :
	public Effect
{
	explicit CriticalDamageDiminution(int turns = 0) :
		Effect("Critical damage diminution", "criticalDamageDiminution", "-50% of critical damage",
		       EffectType::Debuff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.criticalDamage = -0.5}
		)
	{}
};

struct DodgeChanceDiminution :
	public Effect
{
	explicit DodgeChanceDiminution(int turns = 0) :
		Effect("Dodge diminution", "dodgeDiminution", "-50% of dodge",
		       EffectType::Debuff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.dodgeChance = -0.5}
		)
	{}
};

struct MagicalArmorDiminution :
	public Effect
{
	explicit MagicalArmorDiminution(int turns = 0) :
		Effect("Magical armor diminution", "magicalArmorDiminution", "-70% of final magical armor",
		       EffectType::Debuff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.finalMagicalArmorPercent = -0.7}
		)
	{}
};

struct MagicalAttackDiminution :
	public Effect
{
	explicit MagicalAttackDiminution(int turns = 0) :
		Effect("Magical attack diminution", "magicalAttackDiminution", "-70% of final magical attack",
		       EffectType::Debuff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.finalMagicalAttackPercent = -0.7}
		)
	{}
};

struct PhysicalArmorDiminution :
	public Effect
{
	explicit PhysicalArmorDiminution(int turns = 0) :
		Effect("Physical armor diminution", "physicalArmorDiminution", "-70% of final physical armor",
		       EffectType::Debuff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.finalPhysicalArmorPercent = -0.7}
		)
	{}
};

struct PhysicalAttackDiminution :
	public Effect
{
	explicit PhysicalAttackDiminution(int turns = 0) :
		Effect("Physical attack diminution", "physicalAttackDiminution", "-70% of final physical attack",
		       EffectType::Debuff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.finalPhysicalAttackPercent = -0.7}
		)
	{}
};

struct SpeedDiminution :
	public Effect
{
	explicit SpeedDiminution(int turns = 0) :
		Effect("Speed diminution", "speedDiminution", "-30% of attackBarPercentage",
		       EffectType::Debuff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.speed = -30}
		)
	{}
};