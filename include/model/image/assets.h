#pragma once

#include <Magick++.h>
#include <array>

#include "utility/enums.h"
#include "model/image/font.h"
#include "model/entity/effect/effect.h"

enum class ImageName {
	HpBarBackground = 0,
	HpBarContentDiff,
	HpBarContent,
	HpBarContentShield,
	HpBarForeground,
	ExpBarBackground,
	ExpBarContent,
	ExpBarForeground,
	TeamBackground,
	AttackTokenLeft,
	AttackTokenRight,
	Font,
	END
};


struct Assets
{
private:
	Assets();

public:
	std::array<Magick::Image, static_cast<int>(ImageName::END)> images;
    std::array<std::array<Magick::Image, 4>, 2> attackNumbers;
	/**
	 * {className: {skinName: [apparitionImages[images]]}
	 */
	std::map<std::string, std::map<std::string, std::vector<std::vector<Magick::Image>>>> skins;
	std::map<std::string, Magick::Image> classTypeIcons;
	std::map<int, rtpAtlasSprite> font;
	std::map<std::string, Magick::Image> effects;
	std::map<std::string, std::vector<Magick::Image>> skillAnimations;

	Magick::Image getEffect(Effect* effect);
	Magick::Image getImage(ImageName imageName);
    Magick::Image getAttackNumber(Enums::Team team, Enums::Slot slot);
	std::vector<Magick::Image> getSkillAnimation(const std::string& skillAnimationName);

	Assets(const Assets&) = delete;
	Assets& operator=(const Assets&) = delete;
	Assets(Assets&&) = delete;
	Assets& operator=(Assets&&) = delete;

	static auto& getInstance()
	{
		static Assets instance;
		return instance;
	}
};
