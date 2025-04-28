#pragma once

#include <vector>
#include <string>
#include <Magick++/Color.h>

#include "model/entity/classType/target.h"
#include "model/entity/textInformation.h"

struct Entity;

struct FightState
{
    FightState();

    float hp;
	float maxHp;

    // 0 to 1f, when it reaches 1f, the entity can attack. Scale the value to add with the max attackBarPercentage of the fight.
    float attackBarPercent;

	// Increase by 1 when a chance check is unsuccessful. Consumed when a chance check is successful by the amount needed (the bonus x ratio).
	int bonusChanceRatio;

    //TODO: Add all entities that participate in the fight. Need to adapt for more events.

    bool active;
    bool turnActive;
    bool counterAttack;
    bool canBeResurrected;

    std::vector<Effect*> effects;
    /**
     * List of our allies in the fight, we are not included. Updated by Participants.
     */
    std::vector<Entity*> allies;
    std::vector<Entity*> opponents;

    std::function<void(TextInformation)> displayStatus = [](const TextInformation&) {};

    std::string ownerID;
    Target* currentTarget{ nullptr };

    void initialize(Entity* user, std::string ownerID = "");
    void reset();

	bool checkChance(float chance, float malus = 0.f, bool useBonus = true);

    [[nodiscard]] bool isDead() const;
    [[nodiscard]] bool isFullHp() const;
    [[nodiscard]] float getHpPercent() const;
    [[nodiscard]] bool isHpAbove(double percent) const;
    [[nodiscard]] bool isHpBelow(double percent) const;

    void removeExpiredEffects();
    int getShield();

    template<typename Base>
    inline bool hasEffect()
    {
        for (Effect* effect : this->effects)
        {
            if (dynamic_cast<Base*>(effect))
                return true;
        }
        return false;
    };

    template<typename Base>
    inline Base* getEffect()
    {
        for (Effect* effect: this->effects)
        {
            if (Base* baseEffect = dynamic_cast<Base*>(effect))
                return baseEffect;
        }
        return nullptr;
    };

    Effect* getEffect(const std::string& name);

	int countBuffs();
	int countDebuffs();
	//int countStuns();
};