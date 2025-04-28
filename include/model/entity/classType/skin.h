#pragma once

#include <utility>

#include "magic_enum.hpp"

#include "utility/enums.h"
#include "utility/utils.h"

struct SkinAnimation
{
	SkinAnimation() = default;
	SkinAnimation(std::string className, std::string skinName, Enums::Type::Animation animation);

	std::string className;
	std::string skinName;
	Enums::Type::Animation animation{ Enums::Type::Animation::Idle };

	/**
	 * Get a link to the animation.
	 * @return A link to the animation.
	 */
	[[nodiscard]] std::string getLink() const;
};

struct Skin
{
	Skin(const SkinAnimation& idle, const SkinAnimation& apparition, const SkinAnimation& hit, const SkinAnimation& death, const SkinAnimation& attack, bool shaking);
	Skin() = default;

	SkinAnimation idle;
	SkinAnimation apparition;
	SkinAnimation hit;
	SkinAnimation death;
	SkinAnimation attack;
	bool shaking{ false };
};
