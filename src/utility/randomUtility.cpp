#include "utility/randomUtility.h"

#include <random>

namespace RandomUtility
{
	int getRandomInt(int min, int max)
	{
		std::random_device os_seed;
		uint_least32_t seed = os_seed();

		int offset = 0;

		if (min < 0)
		{
			max -= min;
			offset = min;
			min = 0;
		}

		std::mt19937 generator(seed);
		std::uniform_int_distribution<uint_least32_t> distribute(min, max);

		return (int) distribute(generator) + offset;
	}

	float getRandomFloat(float min, float max)
	{
		std::random_device os_seed;
		uint_least32_t seed = os_seed();

		float offset = 0;

		if (min < 0)
		{
			max -= min;
			offset = min;
			min = 0;
		}

		std::mt19937 generator(seed);
		std::uniform_real_distribution<float> distribute(min, max);

		return distribute(generator) + offset;
	}

	bool hasChance(float chance)
	{
		return getRandomFloat(0.f, 1.f) <= chance;
	}
}
