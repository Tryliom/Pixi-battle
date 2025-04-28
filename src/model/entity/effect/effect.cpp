#include "model/entity/effect/effect.h"

Effect::Effect(std::string name, std::string emojiName, std::string description, EffectType type, EffectUpdateTurn updateTurn, bool stackable, int turns, Stat stat)
{
    this->name = std::move(name);
    this->description = std::move(description);
    this->emojiName = std::move(emojiName);
    this->type = type;
    this->updateTurn = updateTurn;
    this->stackable = stackable;
    this->turns = turns;
    this->stat = stat;
}

void Effect::onStartTurn(Entity* user)
{
    this->canTurnBeReduced = true;

    if (this->updateTurn == EffectUpdateTurn::Start)
    {
        this->reduceTurns();
    }
}

void Effect::onEndTurn(Entity* user, Skill* attackUsed)
{
    if (this->updateTurn == EffectUpdateTurn::End && this->canTurnBeReduced)
    {
        this->reduceTurns();
    }
}

void Effect::reduceTurns()
{
    if (this->turns > 0)
        this->turns--;
}

bool Effect::isExpired()
{
    return this->turns == 0;
}

bool Effect::isBetter(Effect* effect)
{
    return this->turns > effect->turns;
}
