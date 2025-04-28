#pragma once

#include <string>
#include <iostream>

#include "model/entity/stat.h"
#include "utility/entityEvents.h"

struct Entity;
struct Skill;
struct Target;

enum class EffectType
{
    Buff = 0,
    Debuff = 1,
    Unique = 2
};

enum class EffectUpdateTurn
{
    Start = 0,
    End = 1
};

struct Effect : public EntityEvents
{
    Effect(std::string name, std::string emojiName, std::string description, EffectType type, EffectUpdateTurn updateTurn, bool stackable, int turns = -1, Stat stat = Stat{});

    std::string name;
    std::string description;
    Stat stat;
    int turns;
    std::string emojiName;
    bool stackable;
    EffectType type;
    EffectUpdateTurn updateTurn;
    bool canTurnBeReduced = false;

    void onStartTurn(Entity* user) override;
    void onEndTurn(Entity* user, Skill* attackUsed) override;

    virtual bool isExpired();
    virtual bool isBetter(Effect* effect);
    void reduceTurns();
};