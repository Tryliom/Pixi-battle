#pragma once

#include <vector>
#include <cmath>
#include <iostream>

#include <dpp/nlohmann/json.hpp>

struct Stat
{
    float physicalAttackFlat{ 0 };
    float physicalAttackPercent{ 0 };

    float magicalAttackFlat{ 0 };
    float magicalAttackPercent{ 0 };

    float healthFlat{0 };
    float healthPercent{0 };

    float speedFlat{ 0 };
    float speedPercent{ 0 };

    float physicalArmorFlat{ 0 };
    float physicalArmorPercent{ 0 };

    float magicalArmorFlat{ 0 };
    float magicalArmorPercent{ 0 };

    float physicalArmorPenetrationFlat{ 0 };
    float physicalArmorPenetrationPercent{ 0 };

    float magicalArmorPenetrationFlat{ 0 };
    float magicalArmorPenetrationPercent{ 0 };

    float lifeSteal{ 0 };
    float counterAttackChance{ 0 };
    float criticalChance{ 0 };
    float criticalDamage{ 0 };
    float dodgeChance{ 0 };
    float successHitChance{ 0 };
    float criticalHitResistance{ 0 };
    float accuracy{ 0 };
    float resistance{ 0 };
    float damageMultiplier{ 0 };
    float healMultiplier{ 0 };
    float damageTakenMultiplier{ 0 };

    float finalHealthPercent{ 0 };
    float finalPhysicalArmorPercent{ 0 };
    float finalMagicalArmorPercent{ 0 };
    float finalSpeedPercent{ 0 };
    float finalPhysicalAttackPercent{ 0 };
    float finalMagicalAttackPercent{ 0 };

    float health = 0;
    float physicalAttack = 0;
    float magicalAttack = 0;
    float speed = 0;
    float physicalArmor = 0;
    float magicalArmor = 0;

    void calculate();

    Stat operator*(Stat other);

	// Used to calculate stat between themselves
	friend void to_json(nlohmann::ordered_json& nlohmann_json_j, const Stat& nlohmann_json_t);
	// Used to saveExistingUsers and loadExistingUsers stat
	friend void to_json(nlohmann::json& nlohmann_json_j, const Stat& nlohmann_json_t);
	friend void from_json(const nlohmann::json& nlohmann_json_j, Stat& nlohmann_json_t);
};

namespace StatUtility
{
    Stat multiply(Stat stat, float n);
    Stat addition(const std::vector<Stat>& list);

    nlohmann::ordered_map<std::string, float> getStatMap(Stat stat);

    /**
     * Format the stat to be display with + and - values
     * @param stat
     * @param returnToLine
     * @return
     */
    std::string formatStat(Stat stat, bool returnToLine = false);
    /**
     * Format the stat to be display with Name of the stat: number
     * @param stat
     * @return
     */
    std::string formatStatDisplay(Stat stat);
}
