#include "model/entity/classType/skin.h"

SkinAnimation::SkinAnimation(std::string className, std::string skinName, Enums::Type::Animation animation)
{
	this->className = std::move(className);
	this->skinName = std::move(skinName);
	this->animation = animation;
}

std::string SkinAnimation::getLink() const
{
	return "../assets/images/classType/" + this->className + "/" + this->skinName + "/" + Utils::toLowerCase((std::string) magic_enum::enum_name(this->animation)) + ".gif";
}

Skin::Skin(const SkinAnimation& idle, const SkinAnimation& apparition, const SkinAnimation& hit, const SkinAnimation& death, const SkinAnimation& attack, bool shaking) :
	idle(idle),
	apparition(apparition),
	hit(hit),
	death(death),
	attack(attack),
	shaking(shaking)
{}