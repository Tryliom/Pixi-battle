#pragma once

#include <map>
#include <string>

namespace Enums
{
	enum class Tier
	{
		Class = 0,
		Common = 1,
		Rare = 2,
		Epic = 3,
		Legendary = 4,
		Unique = 5,
		Unknown = 6
	};

	enum class Team
	{
		Left = 1,
		Right = -1,
		Unknown = 0
	};

	enum class Slot
	{
		TopLeft = 0,
		TopRight = 1,
		BottomLeft = 2,
		BottomRight = 3,
		Unknown = 4
	};

	enum class FightConstants
	{
		FightWidth = 96 + 102 * 4 + 5,
		FightHeight = 138 * 2 + 10 + 32 + 20 + 40,
		ParticipantMaxWidth = 102,
		ParticipantMaxHeight = 138,
		CharacterMaxHeight = 64,
		CharacterMaxWidth = 100,
		effectSize = 16,
		effectOffset = 2
	};

	namespace Type
	{
		enum class Entity
		{
			Player = 0,
			Monster = 1,
			Unknown = 2
		};

		enum class Line
		{
			Back = 0,
			Front = 1,
			Unknown = 2
		};

		enum class Cost
		{
			Health = 0,
			None = 1
		};

		enum class Damage
		{
			Physical = 0,
			Magical = 1,
			None = 2,
			True = 3
		};

		enum class Target
		{
			Self = 0,
			AllEnemies = 1,
			AllAllies = 2,
			SingleEnemy = 3,
			SingleAlly = 4,
			SingleDeadAlly = 5,
			SingleDeadEnemy = 6,
			AllDeadAllies = 7,
			AllDeadEnemies = 8,
			SingleEmptyAlly = 9,
			SingleEmptyEnemy = 10
		};

		enum class Spell
		{
			Damage = 0,
			Status = 1,
			Summon = 2
		};

		enum class Improvement
		{
			Power = 0,
			Cooldown = 1,
			Stat = 2,
			Special = 3,
			Choice = 4,
			EffectChance = 5,
			TurnEffect = 6,
			DamagePercent = 7,
			Stack = 8,
			MaxStack = 9,
			Percent = 10
		};

		enum class Animation
		{
			Idle = 0,
			Apparition = 1,
			Hit = 2,
			Death = 3,
			Attack = 4
		};
	}

	static std::map<Enums::Type::Target, std::string> targetToString = {
		{ Enums::Type::Target::Self, "Self" },
		{ Enums::Type::Target::AllEnemies, "All Enemies" },
		{ Enums::Type::Target::AllAllies, "All Allies" },
		{ Enums::Type::Target::SingleEnemy, "Single Enemy" },
		{ Enums::Type::Target::SingleAlly, "Single Ally" },
		{ Enums::Type::Target::SingleDeadAlly, "Single Dead Ally" },
		{ Enums::Type::Target::SingleDeadEnemy, "Single Dead Enemy" },
		{ Enums::Type::Target::AllDeadAllies, "All Dead Allies" },
		{ Enums::Type::Target::AllDeadEnemies, "All Dead Enemies" },
		{ Enums::Type::Target::SingleEmptyAlly, "Single Empty Ally" },
		{ Enums::Type::Target::SingleEmptyEnemy, "Single Empty Enemy" }
	};

	static std::map<Enums::Type::Damage, std::string> damageToString = {
		{ Enums::Type::Damage::Physical, "Physical" },
		{ Enums::Type::Damage::Magical, "Magical" },
		{ Enums::Type::Damage::None, "None" },
		{ Enums::Type::Damage::True, "True" }
	};
}
