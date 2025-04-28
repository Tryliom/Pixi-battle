#pragma once

namespace RandomUtility
{
	int getRandomInt(int min, int max);
	float getRandomFloat(float min, float max);
	/**
	 * @brief Check if a random float is less than the given chance (between 0 and 1)
	 * @param chance The chance to check
	 * @return true if the random float is less than the given chance
	 */
	bool hasChance(float chance);
}