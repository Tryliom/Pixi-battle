#pragma once

#include <string>
#include <Magick++/Color.h>

#include "model/fight/position.h"

enum class InformationType
{
    Status,
    HealthDifference,
};

struct TextInformation
{
    TextInformation(const std::string& text, InformationType type, const Magick::Color& color, int maxFrame = 20);

    std::string text;
    InformationType type;
    Magick::Color color;
	Position offset{ Position(0, 0) };

    int currentFrame { 0 };
    int maxFrame { 20 };
	double fontSize { 1 };
	double maxFontSize { 14 };

    void nextFrame();
    [[nodiscard]] bool isFinished() const;
    std::string getText();
};

// Define a lot of texts types
namespace Status
{
    struct Dodge :
            TextInformation
    {
        Dodge() :
                TextInformation("Missed", InformationType::Status, Magick::Color(199, 199, 199))
        {}
    };

    struct Ressurection :
            TextInformation
    {
        Ressurection() :
                TextInformation("Resurrected", InformationType::Status, Magick::Color(199, 199, 199))
        {}
    };

    struct Death :
            TextInformation
    {
        Death() :
                TextInformation("Died", InformationType::Status, Magick::Color(199, 199, 199))
        {}
    };

    struct Counter :
            TextInformation
    {
        Counter() :
                TextInformation("Counter", InformationType::Status, Magick::Color(199, 199, 199))
        {}
    };

    struct ExtraTurn :
            TextInformation
    {
        ExtraTurn() :
                TextInformation("Extra turn", InformationType::Status, Magick::Color(255, 249, 62))
        {}
    };

    struct AbsorbShield :
            TextInformation
    {
        AbsorbShield() :
                TextInformation("Absorbed", InformationType::Status, Magick::Color(199, 199, 199))
        {}
    };

    struct Burn :
            TextInformation
    {
        Burn() :
                TextInformation("Burned", InformationType::Status, Magick::Color(199, 199, 199))
        {}
    };

    struct Bleeding :
            TextInformation
    {
        Bleeding() :
                TextInformation("Bleeding", InformationType::Status, Magick::Color(199, 199, 199))
        {}
    };

    struct Poison :
            TextInformation
    {
        Poison() :
                TextInformation("Poisoned", InformationType::Status, Magick::Color(199, 199, 199))
        {}
    };
}

namespace HealthStatus
{
    struct Heal :
            TextInformation
    {
        explicit Heal(int amount) :
                TextInformation("+" + std::to_string(abs(amount)), InformationType::HealthDifference, Magick::Color(134, 225, 112))
        {}
    };

    struct PhysicalDamage :
            TextInformation
    {
        explicit PhysicalDamage(int amount) :
                TextInformation("-" + std::to_string(abs(amount)), InformationType::HealthDifference, Magick::Color(223, 126, 112))
        {}
    };

    struct CriticalPhysicalDamage :
            TextInformation
    {
        explicit CriticalPhysicalDamage(int amount) :
                TextInformation("-" + std::to_string(abs(amount)), InformationType::HealthDifference, Magick::Color(221, 63, 63))
        {}
    };

    struct MagicalDamage :
            TextInformation
    {
        explicit MagicalDamage(int amount) :
                TextInformation("-" + std::to_string(abs(amount)), InformationType::HealthDifference, Magick::Color(112, 169, 223))
        {}
    };

    struct CriticalMagicalDamage :
            TextInformation
    {
        explicit CriticalMagicalDamage(int amount) :
                TextInformation("-" + std::to_string(abs(amount)), InformationType::HealthDifference, Magick::Color(63, 102, 219))
        {}
    };

	struct TrueDamage :
		TextInformation
	{
		explicit TrueDamage(int amount) :
			TextInformation("-" + std::to_string(abs(amount)), InformationType::HealthDifference, Magick::Color("white"))
		{}
	};

    struct DodgeDamage :
            TextInformation
    {
        explicit DodgeDamage(int amount) :
                TextInformation("-" + std::to_string(abs(amount)), InformationType::HealthDifference, Magick::Color(133, 133, 133))
        {}
    };
}