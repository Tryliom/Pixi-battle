#pragma once

#include "model/entity/stat.h"

struct EvolutionaryFactor
{
    explicit EvolutionaryFactor(Stat* factor = nullptr);

    Stat factor;

    void randomize();

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(EvolutionaryFactor, factor)
};
