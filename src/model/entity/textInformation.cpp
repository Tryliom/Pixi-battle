#include "model/entity/textInformation.h"

TextInformation::TextInformation(const std::string& text, InformationType type, const Magick::Color& color, int maxFrame)
{
    this->text = text;
    this->type = type;
    this->color = color;
    this->maxFrame = maxFrame;

	if (type == InformationType::HealthDifference)
	{
		this->maxFontSize = 20;
	}
	else
	{
		this->maxFontSize = 15;
	}

	this->fontSize = maxFontSize;
}

void TextInformation::nextFrame()
{
    if (currentFrame < maxFrame)
    {
        currentFrame++;
    }
}

bool TextInformation::isFinished() const
{
    return currentFrame >= maxFrame;
}

std::string TextInformation::getText()
{
    float progress = (float) currentFrame / (float) maxFrame;

    if (progress < 0.25f)
    {
		fontSize = (progress * 4.f * (float) maxFontSize);
    }

	if (progress > 0.75f)
    {
		fontSize = ((1.f - progress) * 4.f * (float) maxFontSize);
	}

	// Magic number, if you put less than 5, it makes magick crash
	if (fontSize <= 5)
	{
		fontSize = 5;
	}

    if (type == InformationType::HealthDifference)
    {
        std::string textToShow = text;

        if (progress < 0.5f)
        {
            std::string sign = text.substr(0, 1);
            int number = std::stoi(text.substr(1, text.size() - 1));

            int numberToShow = (int) ((float) number * progress * 2.f);
            textToShow = sign + std::to_string(numberToShow);
        }

        return textToShow;
    }

    return text;
}
