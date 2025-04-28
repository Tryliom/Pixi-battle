#pragma once

#include "model/entity/effect/effect.h"

struct AccuracyAugmentation :
	public Effect
{
	explicit AccuracyAugmentation(int turns = 0) :
		Effect("Accuracy augmentation", "accuracyAugmentation", "+50% of accuracy",
		       EffectType::Buff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.accuracy = 0.5}
		)
	{}
};

struct ArmorAugmentation :
	public Effect
{
	explicit ArmorAugmentation(int turns = 0) :
		Effect("Armor augmentation", "armorAugmentation", "+40% of final physical and magic armor",
		       EffectType::Buff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.finalPhysicalArmorPercent = 0.4, .finalMagicalArmorPercent = 0.4}
		)
	{}
};

struct AttackAugmentation :
	public Effect
{
	explicit AttackAugmentation(int turns = 0) :
		Effect("Attack augmentation", "attackAugmentation", "+40% of final physical and magic attack",
		       EffectType::Buff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.finalPhysicalAttackPercent = 0.4, .finalMagicalAttackPercent = 0.4}
		)
	{}
};

struct CriticalChanceAugmentation :
	public Effect
{
	explicit CriticalChanceAugmentation(int turns = 0) :
		Effect("Critical chance augmentation", "criticalChanceAugmentation", "+50% of critical chance",
		       EffectType::Buff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.criticalChance = 0.5}
		)
	{}
};

struct CriticalDamageAugmentation :
	public Effect
{
	explicit CriticalDamageAugmentation(int turns = 0) :
		Effect("Critical damage augmentation", "criticalDamageAugmentation", "+50% of critical damage",
		       EffectType::Buff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.criticalDamage = 0.5}
		)
	{}
};

struct DodgeChanceAugmentation :
	public Effect
{
	explicit DodgeChanceAugmentation(int turns = 0) :
		Effect("Dodge augmentation", "dodgeAugmentation", "+50% of dodge",
		       EffectType::Buff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.dodgeChance = 0.5}
		)
	{}
};

struct MagicalArmorAugmentation :
	public Effect
{
	explicit MagicalArmorAugmentation(int turns = 0) :
		Effect("Magical armor augmentation", "magicalArmorAugmentation", "+70% of final magical armor",
		       EffectType::Buff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.finalMagicalArmorPercent = 0.7}
		)
	{}
};

struct MagicalAttackAugmentation :
	public Effect
{
	explicit MagicalAttackAugmentation(int turns = 0) :
		Effect("Magical attack augmentation", "magicalAttackAugmentation", "+70% of final magical attack",
		       EffectType::Buff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.finalMagicalAttackPercent = 0.7}
		)
	{}
};

struct PhysicalArmorAugmentation :
	public Effect
{
	explicit PhysicalArmorAugmentation(int turns = 0) :
		Effect("Physical armor augmentation", "physicalArmorAugmentation", "+70% of final physical armor",
		       EffectType::Buff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.finalPhysicalArmorPercent = 0.7}
		)
	{}
};

struct PhysicalAttackAugmentation :
	public Effect
{
	explicit PhysicalAttackAugmentation(int turns = 0) :
		Effect("Physical attack augmentation", "physicalAttackAugmentation", "+70% of final physical attack",
		       EffectType::Buff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.finalPhysicalAttackPercent = 0.7}
		)
	{}
};

struct SpeedAugmentation :
	public Effect
{
	explicit SpeedAugmentation(int turns = 0) :
		Effect("Speed augmentation", "speedAugmentation", "+30% of attackBarPercentage",
		       EffectType::Buff, EffectUpdateTurn::End,
		       false, turns,
		       Stat{.speed = 30}
		)
	{}
};