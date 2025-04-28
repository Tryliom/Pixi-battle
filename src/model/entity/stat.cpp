#include <regex>
#include <iostream>

#include "model/entity/stat.h"
#include "utility/utils.h"

using json = nlohmann::json;

void Stat::calculate()
{
    this->health = std::round(this->healthFlat * (1.f + this->healthPercent) * (1.f + this->finalHealthPercent));
    this->physicalAttack = std::round(this->physicalAttackFlat * (1.f + this->physicalAttackPercent) * (1.f + this->finalPhysicalAttackPercent));
    this->magicalAttack = std::round(this->magicalAttackFlat * (1 + this->magicalAttackPercent) * (1.f + this->finalMagicalAttackPercent));
    this->speed = std::round(this->speedFlat * (1.f + this->speedPercent) * (1.f + this->finalSpeedPercent));
    this->physicalArmor = std::round(this->physicalArmorFlat * (1.f + this->physicalArmorPercent) * (1.f + this->finalPhysicalArmorPercent));
    this->magicalArmor = std::round(this->magicalArmorFlat * (1.f + this->magicalArmorPercent) * (1.f + this->finalMagicalArmorPercent));
}

void to_json(nlohmann::ordered_json& nlohmann_json_j, const Stat& nlohmann_json_t)
{
	nlohmann_json_j["health"] = nlohmann_json_t.health;
	nlohmann_json_j["physicalAttack"] = nlohmann_json_t.physicalAttack;
	nlohmann_json_j["magicalAttack"] = nlohmann_json_t.magicalAttack;
	nlohmann_json_j["attackBarPercentage"] = nlohmann_json_t.speed;
	nlohmann_json_j["physicalArmor"] = nlohmann_json_t.physicalArmor;
	nlohmann_json_j["magicalArmor"] = nlohmann_json_t.magicalArmor;

	nlohmann_json_j["healthFlat"] = nlohmann_json_t.healthFlat;
	nlohmann_json_j["healthPercent"] = nlohmann_json_t.healthPercent;
	nlohmann_json_j["finalHealthPercent"] = nlohmann_json_t.finalHealthPercent;
	nlohmann_json_j["physicalAttackFlat"] = nlohmann_json_t.physicalAttackFlat;
	nlohmann_json_j["physicalAttackPercent"] = nlohmann_json_t.physicalAttackPercent;
	nlohmann_json_j["finalPhysicalAttackPercent"] = nlohmann_json_t.finalPhysicalAttackPercent;
	nlohmann_json_j["magicalAttackFlat"] = nlohmann_json_t.magicalAttackFlat;
	nlohmann_json_j["magicalAttackPercent"] = nlohmann_json_t.magicalAttackPercent;
	nlohmann_json_j["finalMagicalAttackPercent"] = nlohmann_json_t.finalMagicalAttackPercent;
	nlohmann_json_j["speedFlat"] = nlohmann_json_t.speedFlat;
	nlohmann_json_j["speedPercent"] = nlohmann_json_t.speedPercent;
	nlohmann_json_j["finalSpeedPercent"] = nlohmann_json_t.finalSpeedPercent;
	nlohmann_json_j["physicalArmorFlat"] = nlohmann_json_t.physicalArmorFlat;
	nlohmann_json_j["physicalArmorPercent"] = nlohmann_json_t.physicalArmorPercent;
	nlohmann_json_j["finalPhysicalArmorPercent"] = nlohmann_json_t.finalPhysicalArmorPercent;
	nlohmann_json_j["magicalArmorFlat"] = nlohmann_json_t.magicalArmorFlat;
	nlohmann_json_j["magicalArmorPercent"] = nlohmann_json_t.magicalArmorPercent;
	nlohmann_json_j["finalMagicalArmorPercent"] = nlohmann_json_t.finalMagicalArmorPercent;
	nlohmann_json_j["criticalChance"] = nlohmann_json_t.criticalChance;
	nlohmann_json_j["criticalDamage"] = nlohmann_json_t.criticalDamage;
	nlohmann_json_j["dodgeChance"] = nlohmann_json_t.dodgeChance;
	nlohmann_json_j["accuracy"] = nlohmann_json_t.accuracy;
	nlohmann_json_j["resistance"] = nlohmann_json_t.resistance;

	nlohmann_json_j["physicalArmorPenetrationFlat"] = nlohmann_json_t.physicalArmorPenetrationFlat;
	nlohmann_json_j["physicalArmorPenetrationPercent"] = nlohmann_json_t.physicalArmorPenetrationPercent;
	nlohmann_json_j["magicalArmorPenetrationFlat"] = nlohmann_json_t.magicalArmorPenetrationFlat;
	nlohmann_json_j["magicalArmorPenetrationPercent"] = nlohmann_json_t.magicalArmorPenetrationPercent;
	nlohmann_json_j["lifeSteal"] = nlohmann_json_t.lifeSteal;
	nlohmann_json_j["counterAttackChance"] = nlohmann_json_t.counterAttackChance;
	nlohmann_json_j["successHitChance"] = nlohmann_json_t.successHitChance;
	nlohmann_json_j["criticalHitResistance"] = nlohmann_json_t.criticalHitResistance;
	nlohmann_json_j["damageMultiplier"] = nlohmann_json_t.damageMultiplier;
	nlohmann_json_j["healMultiplier"] = nlohmann_json_t.healMultiplier;
	nlohmann_json_j["damageTakenMultiplier"] = nlohmann_json_t.damageTakenMultiplier;
}

void to_json(nlohmann::json& nlohmann_json_j, const Stat& nlohmann_json_t)
{
	if (nlohmann_json_t.healthFlat != 0)
		nlohmann_json_j["healthFlat"] = nlohmann_json_t.healthFlat;
	if (nlohmann_json_t.healthPercent != 0)
		nlohmann_json_j["healthPercent"] = nlohmann_json_t.healthPercent;
	if (nlohmann_json_t.finalHealthPercent != 0)
		nlohmann_json_j["finalHealthPercent"] = nlohmann_json_t.finalHealthPercent;
	if (nlohmann_json_t.physicalAttackFlat != 0)
		nlohmann_json_j["physicalAttackFlat"] = nlohmann_json_t.physicalAttackFlat;
	if (nlohmann_json_t.physicalAttackPercent != 0)
		nlohmann_json_j["physicalAttackPercent"] = nlohmann_json_t.physicalAttackPercent;
	if (nlohmann_json_t.finalPhysicalAttackPercent != 0)
		nlohmann_json_j["finalPhysicalAttackPercent"] = nlohmann_json_t.finalPhysicalAttackPercent;
	if (nlohmann_json_t.magicalAttackFlat != 0)
		nlohmann_json_j["magicalAttackFlat"] = nlohmann_json_t.magicalAttackFlat;
	if (nlohmann_json_t.magicalAttackPercent != 0)
		nlohmann_json_j["magicalAttackPercent"] = nlohmann_json_t.magicalAttackPercent;
	if (nlohmann_json_t.finalMagicalAttackPercent != 0)
		nlohmann_json_j["finalMagicalAttackPercent"] = nlohmann_json_t.finalMagicalAttackPercent;
	if (nlohmann_json_t.speedFlat != 0)
		nlohmann_json_j["speedFlat"] = nlohmann_json_t.speedFlat;
	if (nlohmann_json_t.speedPercent != 0)
		nlohmann_json_j["speedPercent"] = nlohmann_json_t.speedPercent;
	if (nlohmann_json_t.finalSpeedPercent != 0)
		nlohmann_json_j["finalSpeedPercent"] = nlohmann_json_t.finalSpeedPercent;
	if (nlohmann_json_t.physicalArmorFlat != 0)
		nlohmann_json_j["physicalArmorFlat"] = nlohmann_json_t.physicalArmorFlat;
	if (nlohmann_json_t.physicalArmorPercent != 0)
		nlohmann_json_j["physicalArmorPercent"] = nlohmann_json_t.physicalArmorPercent;
	if (nlohmann_json_t.finalPhysicalArmorPercent != 0)
		nlohmann_json_j["finalPhysicalArmorPercent"] = nlohmann_json_t.finalPhysicalArmorPercent;
	if (nlohmann_json_t.magicalArmorFlat != 0)
		nlohmann_json_j["magicalArmorFlat"] = nlohmann_json_t.magicalArmorFlat;
	if (nlohmann_json_t.magicalArmorPercent != 0)
		nlohmann_json_j["magicalArmorPercent"] = nlohmann_json_t.magicalArmorPercent;
	if (nlohmann_json_t.finalMagicalArmorPercent != 0)
		nlohmann_json_j["finalMagicalArmorPercent"] = nlohmann_json_t.finalMagicalArmorPercent;
	if (nlohmann_json_t.criticalChance != 0)
		nlohmann_json_j["criticalChance"] = nlohmann_json_t.criticalChance;
	if (nlohmann_json_t.criticalDamage != 0)
		nlohmann_json_j["criticalDamage"] = nlohmann_json_t.criticalDamage;
	if (nlohmann_json_t.dodgeChance != 0)
		nlohmann_json_j["dodgeChance"] = nlohmann_json_t.dodgeChance;
	if (nlohmann_json_t.accuracy != 0)
		nlohmann_json_j["accuracy"] = nlohmann_json_t.accuracy;
	if (nlohmann_json_t.resistance != 0)
		nlohmann_json_j["resistance"] = nlohmann_json_t.resistance;

	if (nlohmann_json_t.physicalArmorPenetrationFlat != 0)
		nlohmann_json_j["physicalArmorPenetrationFlat"] = nlohmann_json_t.physicalArmorPenetrationFlat;
	if (nlohmann_json_t.physicalArmorPenetrationPercent != 0)
		nlohmann_json_j["physicalArmorPenetrationPercent"] = nlohmann_json_t.physicalArmorPenetrationPercent;
	if (nlohmann_json_t.magicalArmorPenetrationFlat != 0)
		nlohmann_json_j["magicalArmorPenetrationFlat"] = nlohmann_json_t.magicalArmorPenetrationFlat;
	if (nlohmann_json_t.magicalArmorPenetrationPercent != 0)
		nlohmann_json_j["magicalArmorPenetrationPercent"] = nlohmann_json_t.magicalArmorPenetrationPercent;
	if (nlohmann_json_t.lifeSteal != 0)
		nlohmann_json_j["lifeSteal"] = nlohmann_json_t.lifeSteal;
	if (nlohmann_json_t.counterAttackChance != 0)
		nlohmann_json_j["counterAttackChance"] = nlohmann_json_t.counterAttackChance;
	if (nlohmann_json_t.successHitChance != 0)
		nlohmann_json_j["successHitChance"] = nlohmann_json_t.successHitChance;
	if (nlohmann_json_t.criticalHitResistance != 0)
		nlohmann_json_j["criticalHitResistance"] = nlohmann_json_t.criticalHitResistance;
	if (nlohmann_json_t.damageMultiplier != 0)
		nlohmann_json_j["damageMultiplier"] = nlohmann_json_t.damageMultiplier;
	if (nlohmann_json_t.healMultiplier != 0)
		nlohmann_json_j["healMultiplier"] = nlohmann_json_t.healMultiplier;
	if (nlohmann_json_t.damageTakenMultiplier != 0)
		nlohmann_json_j["damageTakenMultiplier"] = nlohmann_json_t.damageTakenMultiplier;
}

void from_json(const json& nlohmann_json_j, Stat& nlohmann_json_t)
{
	if (nlohmann_json_j.find("healthFlat") != nlohmann_json_j.end())
		nlohmann_json_t.healthFlat = nlohmann_json_j["healthFlat"];
	if (nlohmann_json_j.find("healthPercent") != nlohmann_json_j.end())
		nlohmann_json_t.healthPercent = nlohmann_json_j["healthPercent"];
	if (nlohmann_json_j.find("finalHealthPercent") != nlohmann_json_j.end())
		nlohmann_json_t.finalHealthPercent = nlohmann_json_j["finalHealthPercent"];

	if (nlohmann_json_j.find("physicalAttackFlat") != nlohmann_json_j.end())
		nlohmann_json_t.physicalAttackFlat = nlohmann_json_j["physicalAttackFlat"];
	if (nlohmann_json_j.find("physicalAttackPercent") != nlohmann_json_j.end())
		nlohmann_json_t.physicalAttackPercent = nlohmann_json_j["physicalAttackPercent"];
	if (nlohmann_json_j.find("finalPhysicalAttackPercent") != nlohmann_json_j.end())
		nlohmann_json_t.finalPhysicalAttackPercent = nlohmann_json_j["finalPhysicalAttackPercent"];

	if (nlohmann_json_j.find("magicalAttackFlat") != nlohmann_json_j.end())
		nlohmann_json_t.magicalAttackFlat = nlohmann_json_j["magicalAttackFlat"];
	if (nlohmann_json_j.find("magicalAttackPercent") != nlohmann_json_j.end())
		nlohmann_json_t.magicalAttackPercent = nlohmann_json_j["magicalAttackPercent"];
	if (nlohmann_json_j.find("finalMagicalAttackPercent") != nlohmann_json_j.end())
		nlohmann_json_t.finalMagicalAttackPercent = nlohmann_json_j["finalMagicalAttackPercent"];

	if (nlohmann_json_j.find("speedFlat") != nlohmann_json_j.end())
		nlohmann_json_t.speedFlat = nlohmann_json_j["speedFlat"];
	if (nlohmann_json_j.find("speedPercent") != nlohmann_json_j.end())
		nlohmann_json_t.speedPercent = nlohmann_json_j["speedPercent"];
	if (nlohmann_json_j.find("finalSpeedPercent") != nlohmann_json_j.end())
		nlohmann_json_t.finalSpeedPercent = nlohmann_json_j["finalSpeedPercent"];

	if (nlohmann_json_j.find("physicalArmorFlat") != nlohmann_json_j.end())
		nlohmann_json_t.physicalArmorFlat = nlohmann_json_j["physicalArmorFlat"];
	if (nlohmann_json_j.find("physicalArmorPercent") != nlohmann_json_j.end())
		nlohmann_json_t.physicalArmorPercent = nlohmann_json_j["physicalArmorPercent"];
	if (nlohmann_json_j.find("finalPhysicalArmorPercent") != nlohmann_json_j.end())
		nlohmann_json_t.finalPhysicalArmorPercent = nlohmann_json_j["finalPhysicalArmorPercent"];

	if (nlohmann_json_j.find("magicalArmorFlat") != nlohmann_json_j.end())
		nlohmann_json_t.magicalArmorFlat = nlohmann_json_j["magicalArmorFlat"];
	if (nlohmann_json_j.find("magicalArmorPercent") != nlohmann_json_j.end())
		nlohmann_json_t.magicalArmorPercent = nlohmann_json_j["magicalArmorPercent"];
	if (nlohmann_json_j.find("finalMagicalArmorPercent") != nlohmann_json_j.end())
		nlohmann_json_t.finalMagicalArmorPercent = nlohmann_json_j["finalMagicalArmorPercent"];

	if (nlohmann_json_j.find("criticalChance") != nlohmann_json_j.end())
		nlohmann_json_t.criticalChance = nlohmann_json_j["criticalChance"];
	if (nlohmann_json_j.find("criticalDamage") != nlohmann_json_j.end())
		nlohmann_json_t.criticalDamage = nlohmann_json_j["criticalDamage"];
	if (nlohmann_json_j.find("dodgeChance") != nlohmann_json_j.end())
		nlohmann_json_t.dodgeChance = nlohmann_json_j["dodgeChance"];
	if (nlohmann_json_j.find("accuracy") != nlohmann_json_j.end())
		nlohmann_json_t.accuracy = nlohmann_json_j["accuracy"];
	if (nlohmann_json_j.find("resistance") != nlohmann_json_j.end())
		nlohmann_json_t.resistance = nlohmann_json_j["resistance"];

	if (nlohmann_json_j.find("physicalArmorPenetrationFlat") != nlohmann_json_j.end())
		nlohmann_json_t.physicalArmorPenetrationFlat = nlohmann_json_j["physicalArmorPenetrationFlat"];
	if (nlohmann_json_j.find("physicalArmorPenetrationPercent") != nlohmann_json_j.end())
		nlohmann_json_t.physicalArmorPenetrationPercent = nlohmann_json_j["physicalArmorPenetrationPercent"];
	if (nlohmann_json_j.find("magicalArmorPenetrationFlat") != nlohmann_json_j.end())
		nlohmann_json_t.magicalArmorPenetrationFlat = nlohmann_json_j["magicalArmorPenetrationFlat"];
	if (nlohmann_json_j.find("magicalArmorPenetrationPercent") != nlohmann_json_j.end())
		nlohmann_json_t.magicalArmorPenetrationPercent = nlohmann_json_j["magicalArmorPenetrationPercent"];

	if (nlohmann_json_j.find("lifeSteal") != nlohmann_json_j.end())
		nlohmann_json_t.lifeSteal = nlohmann_json_j["lifeSteal"];
	if (nlohmann_json_j.find("counterAttackChance") != nlohmann_json_j.end())
		nlohmann_json_t.counterAttackChance = nlohmann_json_j["counterAttackChance"];
	if (nlohmann_json_j.find("successHitChance") != nlohmann_json_j.end())
		nlohmann_json_t.successHitChance = nlohmann_json_j["successHitChance"];
	if (nlohmann_json_j.find("criticalHitResistance") != nlohmann_json_j.end())
		nlohmann_json_t.criticalHitResistance = nlohmann_json_j["criticalHitResistance"];
	if (nlohmann_json_j.find("damageMultiplier") != nlohmann_json_j.end())
		nlohmann_json_t.damageMultiplier = nlohmann_json_j["damageMultiplier"];
	if (nlohmann_json_j.find("healMultiplier") != nlohmann_json_j.end())
		nlohmann_json_t.healMultiplier = nlohmann_json_j["healMultiplier"];
	if (nlohmann_json_j.find("damageTakenMultiplier") != nlohmann_json_j.end())
		nlohmann_json_t.damageTakenMultiplier = nlohmann_json_j["damageTakenMultiplier"];
}

Stat Stat::operator*(Stat other)
{
    json currentStat = Stat{};
    json json1 = other;
    json json2 = *this;

    for (auto& [ key, value ]: currentStat.items())
    {
        currentStat.at(key) = (float) json1.at(key) * (float) json2.at(key);
    }

    return currentStat.get<Stat>();
}

namespace StatUtility
{
    Stat multiply(Stat stat, float n)
    {
	    nlohmann::ordered_json currentStat = stat;

        for (auto& [ key, value ]: currentStat.items())
        {
            currentStat.at(key) = static_cast<float>(value) * n;
        }

        return currentStat.get<Stat>();
    }

    Stat addition(const std::vector<Stat>& list)
    {
	    nlohmann::ordered_json currentStat = Stat{};

        for (Stat stat: list)
        {
	        nlohmann::ordered_json statJson = stat;

            for (auto& [ key, value ]: currentStat.items())
            {
                currentStat.at(key) = (float) currentStat.at(key) + (float) statJson.at(key);
            }
        }

        return currentStat.get<Stat>();
    }

    nlohmann::ordered_map<std::string, float> getStatMap(Stat stat)
    {
        nlohmann::ordered_json currentStat = stat;
        nlohmann::ordered_map<std::string, float> statMap;

        for (auto& [ key, value ]: currentStat.items())
        {
            if (value == 0) continue;
            statMap[key] = (float) value;
        }

        return statMap;
    }

    std::string formatStat(Stat stat, bool returnToLine)
    {
        std::string str;
        std::smatch match;

        for (auto& [ key, value ]: getStatMap(stat))
        {
            std::string name = key;
			std::string suffix;
	        auto valueFloat = (float) value;

            if (str.length() > 0)
            {
                if (returnToLine)
                {
                    str += "\n";
                }
                else
                {
                    str += ", ";
                }
            }

            if (valueFloat > 0)
            {
                str += "+";
            }

            if (Utils::contains(key, "Chance|Percent|critical|Multiplier|lifeSteal"))
            {
	            valueFloat *= 100.f;
	            suffix = "%";
            }

	        str += std::to_string(static_cast<int>(valueFloat)) + suffix;

            str += " of ";
            name = std::regex_replace(key, std::regex("Percent|Flat"), "");

            for (char i: name)
            {
                if (isupper(i))
                {
                    str += " ";
                }
                str.push_back((char) tolower(i));
            }
        }

        return str;
    }

    std::string formatStatDisplay(Stat stat)
    {
        std::string str;
        std::smatch match;

        for (auto& [ key, value ]: getStatMap(stat))
        {
            std::string name = key;
            if (str.length() > 0)
            {
                str += "\n";
            }

            name = std::regex_replace(key, std::regex("Percent|Flat"), "");
            std::string completeName;
			std::string suffix;
			auto valueFloat = (float) value;
            bool first = true;

            for (char c: name)
            {
                if (isupper(c))
                {
                    completeName += " ";
                }

                if (first)
                {
                    first = false;
                    completeName.push_back((char) toupper(c));
                }
                else
                {
                    completeName.push_back((char) tolower(c));
                }
            }

            if (Utils::contains(key, "Chance|Percent|critical|Multiplier|lifeSteal|accuracy"))
            {
	            valueFloat *= 100;
	            suffix = "%";
            }

	        str += completeName + ": " + std::to_string(static_cast<int>(valueFloat)) + suffix;
        }

        return str;
    }
}
