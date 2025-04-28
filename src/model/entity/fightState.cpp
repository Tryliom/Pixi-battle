#include "model/entity/fightState.h"

#include <utility>

#include "model/entity/entity.h"
#include "model/entity/effect/effects/shield.h"
#include "utility/randomUtility.h"

FightState::FightState()
{
    this->hp = 0;
    this->maxHp = 0;
    this->attackBarPercent = 0.f;
    this->active = false;
    this->turnActive = false;
    this->counterAttack = false;
    this->canBeResurrected = false;
}

void FightState::initialize(Entity* user, std::string ownerID)
{
    user->updateSkills();
    user->updateStats();

    this->hp = user->stats.health;
    this->maxHp = user->stats.health;
    this->attackBarPercent = 0.f;
	this->bonusChanceRatio = 0;
    this->effects = {};
    this->active = true;
    this->canBeResurrected = true;
    this->ownerID = std::move(ownerID);
}

void FightState::reset()
{
    this->hp = 0;
    this->maxHp = 0;
    this->attackBarPercent = 0.0;
	this->bonusChanceRatio = 0;
	this->effects = {};
	this->active = false;
	this->turnActive = false;
	this->counterAttack = false;
	this->canBeResurrected = false;
	this->ownerID = "";
	this->currentTarget = nullptr;
}

bool FightState::isDead() const
{
    return this->hp <= 0;
}

bool FightState::isFullHp() const
{
    return this->hp == this->maxHp;
}

float FightState::getHpPercent() const
{
    return this->hp / this->maxHp;
}

bool FightState::isHpAbove(double percent) const
{
    return this->getHpPercent() > percent;
}

bool FightState::isHpBelow(double percent) const
{
    return this->getHpPercent() < percent;
}

void FightState::removeExpiredEffects()
{
    for (int i = 0; i < this->effects.size(); i++)
    {
        if (this->effects[i]->isExpired())
        {
            this->effects.erase(this->effects.begin() + i);
            i--;
        }
    }
}

int FightState::getShield()
{
    auto* shield = this->getEffect<Shield>();
    if (shield != nullptr)
    {
        return shield->value;
    }

    return 0;
}

Effect* FightState::getEffect(const std::string& name)
{
    for (Effect* effect: this->effects)
    {
        if (effect->name == name)
        {
            return effect;
        }
    }
    return nullptr;
}

int FightState::countBuffs()
{
	int count = 0;

	for (Effect* effect: this->effects)
	{
		if (effect->type == EffectType::Buff)
		{
			count++;
		}
	}

	return count;
}

int FightState::countDebuffs()
{
	int count = 0;

	for (Effect* effect: this->effects)
	{
		if (effect->type == EffectType::Debuff)
		{
			count++;
		}
	}

	return count;
}

bool FightState::checkChance(float chance, float malus, bool useBonus)
{
	float random = RandomUtility::getRandomFloat(0.f, 1.f);

	if (random <= chance - malus)
	{
		return true;
	}
	else
	{
		int bonusNeeded = static_cast<int>(std::ceil((malus + random) / chance)) - 1;

		if (useBonus && this->bonusChanceRatio >= bonusNeeded)
		{
			this->bonusChanceRatio -= bonusNeeded;
			return true;
		}
		else
		{
			this->bonusChanceRatio++;
			return false;
		}
	}
}

