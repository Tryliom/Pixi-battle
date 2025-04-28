#include "model/entity/evolutionaryFactor.h"

#include "utility/randomUtility.h"

EvolutionaryFactor::EvolutionaryFactor(Stat* factor)
{
    if (factor != nullptr)
    {
        this->factor = *factor;
    }
    else
    {
        this->randomize();
    }
}

void EvolutionaryFactor::randomize()
{
    this->factor = Stat{
        .physicalAttackPercent = RandomUtility::getRandomFloat(0, 0.2f),
        .magicalAttackPercent = RandomUtility::getRandomFloat(0, 0.2f),
        .healthPercent = RandomUtility::getRandomFloat(0, 0.2f),
        .speedPercent = RandomUtility::getRandomFloat(0, 0.2f),
        .physicalArmorPercent = RandomUtility::getRandomFloat(0, 0.2f),
        .magicalArmorPercent = RandomUtility::getRandomFloat(0, 0.2f),
    };

	if (this->factor.physicalAttackPercent < 0.01f)
		this->factor.physicalAttackPercent = 0;
	if (this->factor.magicalAttackPercent < 0.01f)
		this->factor.magicalAttackPercent = 0;
	if (this->factor.healthPercent < 0.01f)
		this->factor.healthPercent = 0;
	if (this->factor.speedPercent < 0.01f)
		this->factor.speedPercent = 0;
	if (this->factor.physicalArmorPercent < 0.01f)
		this->factor.physicalArmorPercent = 0;
	if (this->factor.magicalArmorPercent < 0.01f)
		this->factor.magicalArmorPercent = 0;
}
