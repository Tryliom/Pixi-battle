#include "model/image/assets.h"

#include "model/entity/classType/classType.h"
#include "manager/classManager.h"
#include "manager/skillManager.h"
#include "model/entity/skill/skill.h"

Assets::Assets()
{
	this->images[static_cast<int>(ImageName::HpBarBackground)] = Magick::Image("../assets/images/hud/hud_life_0.png");
	this->images[static_cast<int>(ImageName::HpBarContentDiff)] = Magick::Image("../assets/images/hud/hud_life_1.png");
	this->images[static_cast<int>(ImageName::HpBarContent)] = Magick::Image("../assets/images/hud/hud_life_2.png");
	this->images[static_cast<int>(ImageName::HpBarContentShield)] = Magick::Image("../assets/images/hud/hud_life_3.png");
	this->images[static_cast<int>(ImageName::HpBarForeground)] = Magick::Image("../assets/images/hud/hud_life_4.png");
	this->images[static_cast<int>(ImageName::ExpBarBackground)] = Magick::Image("../assets/images/hud/experience_bar_0.png");
	this->images[static_cast<int>(ImageName::ExpBarContent)] = Magick::Image("../assets/images/hud/experience_bar_1.png");
	this->images[static_cast<int>(ImageName::ExpBarForeground)] = Magick::Image("../assets/images/hud/experience_bar_2.png");
	this->images[static_cast<int>(ImageName::TeamBackground)] = Magick::Image("../assets/images/background/background_teams.png");
	this->images[static_cast<int>(ImageName::AttackTokenLeft)] = Magick::Image("../assets/images/hud/attack_token_left.png");
	this->images[static_cast<int>(ImageName::AttackTokenRight)] = Magick::Image("../assets/images/hud/attack_token_right.png");
	this->images[static_cast<int>(ImageName::Font)] = Magick::Image(ATLAS_FONT_IMAGE_PATH);

    this->attackNumbers[0][0] = Magick::Image("../assets/images/hud/attack_numbers/sprite_0.png");
    this->attackNumbers[0][1] = Magick::Image("../assets/images/hud/attack_numbers/sprite_1.png");
    this->attackNumbers[0][2] = Magick::Image("../assets/images/hud/attack_numbers/sprite_2.png");
    this->attackNumbers[0][3] = Magick::Image("../assets/images/hud/attack_numbers/sprite_3.png");
    this->attackNumbers[1][0] = Magick::Image("../assets/images/hud/attack_numbers/sprite_4.png");
    this->attackNumbers[1][1] = Magick::Image("../assets/images/hud/attack_numbers/sprite_5.png");
    this->attackNumbers[1][2] = Magick::Image("../assets/images/hud/attack_numbers/sprite_6.png");
    this->attackNumbers[1][3] = Magick::Image("../assets/images/hud/attack_numbers/sprite_7.png");

	this->skins = {};
	for (ClassType* classType: ClassManager::getClasses())
	{
		std::map<std::string, std::vector<std::vector<Magick::Image>>> classSkins = {};
		std::vector<std::vector<Magick::Image>> skinAnimations = {
			std::vector<Magick::Image>(),
			std::vector<Magick::Image>(),
			std::vector<Magick::Image>(),
			std::vector<Magick::Image>(),
			std::vector<Magick::Image>()
		};
		Magick::readImages(&skinAnimations[0], classType->skin.idle.getLink());
		Magick::readImages(&skinAnimations[1], classType->skin.apparition.getLink());
		Magick::readImages(&skinAnimations[2], classType->skin.hit.getLink());
		Magick::readImages(&skinAnimations[3], classType->skin.death.getLink());
		Magick::readImages(&skinAnimations[4], classType->skin.attack.getLink());

		classSkins["default"] = skinAnimations;
		this->skins[classType->id] = classSkins;
		this->classTypeIcons[classType->id] = Magick::Image("../assets/images/icon/classType/" + classType->emojiName + ".png");
	}

	this->font = {};
	for (rtpAtlasSprite sprite: rtpDescBoldFonts)
	{
		this->font[sprite.charValue] = sprite;
	}

	this->effects = {};

	// Get all active skills and load their skill animation
	for (SkillName skillName : SkillManager::getActiveSkills())
	{
		ActiveSkill* skill = dynamic_cast<ActiveSkill*>(SkillFactory::getSkill(skillName).get());
		std::string name = skill->animation.name + "_";

		for (AnimationStep step : skill->animation.steps)
		{
			std::vector<Magick::Image> animation = {};

			Magick::readImages(&animation, "../assets/images/skill/" + name + step.getName() + ".gif");

			this->skillAnimations[name + step.getName()] = animation;
		}
	}
}

Magick::Image Assets::getEffect(Effect* effect)
{
	if (this->effects.find(effect->name) == this->effects.end())
	{
		this->effects[effect->name] = Magick::Image("../assets/images/effect/" + effect->emojiName + ".png");
	}

	return this->effects[effect->name];
}

Magick::Image Assets::getImage(ImageName imageName)
{
	return this->images[static_cast<int>(imageName)];
}

Magick::Image Assets::getAttackNumber(Enums::Team team, Enums::Slot slot)
{
    return this->attackNumbers[team == Enums::Team::Left ? 0 : 1][static_cast<int>(slot)];
}

std::vector<Magick::Image> Assets::getSkillAnimation(const std::string& skillAnimationName)
{
	return this->skillAnimations[skillAnimationName];
}
