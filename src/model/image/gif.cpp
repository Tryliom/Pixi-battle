#include "model/image/gif.h"

#include "model/fight/participant.h"
#include "model/entity/entity.h"
#include "model/entity/fightState.h"
#include "model/entity/classType/classType.h"
#include "utility/randomUtility.h"

Gif::Gif(int width, int height)
{
	this->width = width;
	this->height = height;
	this->initializeNewImage();
}

Gif::Gif()
{
	this->width = (int) Enums::FightConstants::FightWidth;
	this->height = (int) Enums::FightConstants::FightHeight;
	this->initializeNewImage();
}

Gif::~Gif()
{
	this->frames.clear();
}

void Gif::addImage()
{
	this->frames.emplace_back(this->currentImage);
}

void Gif::initializeNewImage()
{
	this->currentImage = Magick::Image(Magick::Geometry(this->width, this->height, 0, 0), Magick::Color(61, 61, 61));
	this->drawImage(Assets::getInstance().getImage(ImageName::TeamBackground), Position(0, 0), false, false);
	this->currentImage.animationDelay(100 / 20);
	this->currentImage.animationIterations(1);
	this->currentImage.magick("gif");
}

void Gif::addAndInitializeNewImage()
{
	this->addImage();
	this->initializeNewImage();
}

std::string Gif::generate()
{
	Magick::Blob blob;

	writeImages(this->frames.begin(), this->frames.end(), &blob);

	return Utils::decode(blob.base64());
}

std::string Gif::generateLastFrame()
{
	Magick::Blob blob;
	Magick::Image lastFrame = this->frames[this->frames.size() - 1];

	lastFrame.magick("png");
	lastFrame.write(&blob);

	return Utils::decode(blob.base64());
}

void Gif::drawText(const Text& text)
{
	Magick::Image textImage(Magick::Geometry(this->width, ATLAS_FONT_SIZE, 0, 0), Magick::Color("transparent"));
	int lastX = 2;

	for (int charIndex: text.text)
	{
		rtpAtlasSprite sprite = Assets::getInstance().font[charIndex];
		Magick::Image charImage = Magick::Image(Assets::getInstance().getImage(ImageName::Font));
		charImage.crop(Magick::Geometry(sprite.sourceWidth, sprite.sourceHeight, sprite.positionX, sprite.positionY));
		textImage.composite(charImage, lastX + sprite.charOffsetX, sprite.charOffsetY, Magick::OverCompositeOp);
		lastX += sprite.charAdvanceX + 1;
	}

	textImage.crop(Magick::Geometry(lastX, ATLAS_FONT_SIZE, 0, 0));
	double ratio = text.fontSize / ATLAS_FONT_SIZE;
	auto geometryRatio = Magick::Geometry(textImage.columns() * ratio, textImage.rows() * ratio);

	textImage.scale(geometryRatio);

	if (text.shadow)
	{
		Magick::Image shadowImage = Magick::Image(textImage);
		Gif::replaceColorSDF(shadowImage, Magick::Color(0, 0, 0));
		this->drawImage(shadowImage, Position(text.position.x - 1, text.position.y + 1), text.xCentered, text.yCentered);
	}

	if (text.color != Magick::Color("white"))
	{
		Gif::replaceColorSDF(textImage, text.color);
	}

	this->drawImage(textImage, Position(text.position.x, text.position.y), text.xCentered, text.yCentered);
}

void Gif::replaceColorSDF(Magick::Image& image, const Magick::Color& replaceColor)
{
	uint width = image.columns();
	uint height = image.rows();
	Magick::PixelPacket* pixels = image.getPixels(0, 0, width, height);

	for (uint column = 0; column <= height; column++)
	{
		for (uint row = 0; row <= width; row++)
		{
			Magick::Color px = image.pixelColor(row, column);
			pixels[column * width + row] = Magick::Color(
				static_cast<int>(replaceColor.redQuantum()),
				static_cast<int>(replaceColor.greenQuantum()),
				static_cast<int>(replaceColor.blueQuantum()),
				static_cast<int>(px.alphaQuantum())
			);
		}
	}
	image.syncPixels();
}

void Gif::drawImage(const Magick::Image& image, Position position, bool xCentered, bool yCentered)
{
	int x = position.x;
	int y = position.y;

	if (xCentered)
	{
		x -= static_cast<int>(image.size().width()) / 2;
	}

	if (yCentered)
	{
		y -= static_cast<int>(image.size().height()) / 2;
	}

	this->currentImage.composite(image, x, y, Magick::OverCompositeOp);
}

void Gif::drawImagePart(Magick::Image image, Position position, double widthPercent, int xOffset)
{
	int x = (int) (position.x - image.columns() / 2.0);
	int y = (int) (position.y - image.rows() / 2.0);

	Magick::Geometry geometry((int) (image.columns() * widthPercent), image.rows());
	geometry.xOff(xOffset);

	if (geometry.width() > 0)
	{
		image.crop(geometry);
		this->currentImage.composite(image, x, y, Magick::OverCompositeOp);
	}
}

void Gif::drawHpBar(const Participant& participant, Position position)
{
	Assets& assets = Assets::getInstance();
    HealthBarAnimation animation = participant.healthBarAnimation;
    auto content = assets.getImage(ImageName::HpBarContent);

	this->drawImage(assets.getImage(ImageName::HpBarBackground), position);
	this->drawImagePart(content, position, animation.currentHealthPercentage, 0);

    if (participant.entity->state->getShield() > 0)
    {
        float xOffset = (float) content.columns() * animation.currentHealthPercentage;
        this->drawImagePart(assets.getImage(ImageName::HpBarContentShield), Position(position.x + xOffset, position.y),
                            1.f - animation.currentHealthPercentage, xOffset);
    }

    if (!animation.isFinished())
    {
		float xOffset = animation.getDifferenceOffset(content.size().width());
        this->drawImagePart(assets.getImage(ImageName::HpBarContentDiff),
							Position(position.x + xOffset, position.y), animation.getDifferenceWidth(), xOffset);
    }

	this->drawImage(assets.getImage(ImageName::HpBarForeground), position);
}

Position Gif::getAttackBarPosition(int index, int tokenWidth)
{
	int y = (int) Enums::FightConstants::FightHeight - (36 / 2 + 36 + 5);
	int startX = 10;
	int spaceX = 5;

	int attackerX = 21;
	int attackerY = ((int) Enums::FightConstants::FightHeight) - 20;

	if (index == 0)
	{
		return {attackerX, attackerY};
	}

	return {startX + (tokenWidth + spaceX) * index + tokenWidth / 2 + spaceX, y};
}

void Gif::drawAttackBar(Participants participants)
{
	Assets& tmpAssets = Assets::getInstance();

	if (participants.turnOrderAnimation.isFinished())
	{
		std::vector<int> participantsTurnOrder = participants.currentTurnOrder;

		for (int i = 0; i < participantsTurnOrder.size(); i++)
		{
			int participantIndex = participantsTurnOrder[i];
			Participant& participant = participants.participants[participantIndex];
			Magick::Image attackToken = Assets::getInstance().getImage(participant.team == Enums::Team::Left ? ImageName::AttackTokenLeft : ImageName::AttackTokenRight);
			Magick::Image attackNumber = tmpAssets.getAttackNumber(participant.team, participant.slot);
			Magick::Geometry atSize = attackToken.size(), anSize = attackNumber.size();

			Position position = this->getAttackBarPosition(i, atSize.width());

			attackToken.composite(tmpAssets.classTypeIcons[participant.entity->classType->id], 4, 3, Magick::OverCompositeOp);

			this->drawImage(attackToken, position);

			if (participants.playerMustPlay() && participants.isAllAnimationsFinished())
			{
				this->drawImage(attackNumber, Position(position.x, position.y - atSize.height() / 2 - anSize.height() / 2 - 1));
			}
		}
	}
	else
	{
		TurnOrderAnimation& animation = participants.turnOrderAnimation;
		float progress = animation.getPercentage();

		for (const auto& transitions: animation.turnOrderPositions)
		{
			Magick::Image attackToken = Assets::getInstance().getImage(transitions.team == Enums::Team::Left ? ImageName::AttackTokenLeft : ImageName::AttackTokenRight);
			Magick::Geometry atSize = attackToken.size();

			attackToken.composite(tmpAssets.classTypeIcons[transitions.classID], 4, 3, Magick::OverCompositeOp);

			Position positionA = this->getAttackBarPosition(transitions.indexA, atSize.width());
			Position positionB = this->getAttackBarPosition(transitions.indexB, atSize.width());

			Position position = positionA + (positionB - positionA) * progress;
			this->drawImage(attackToken, position);
		}
	}
}

void Gif::drawEntity(const Participant& participant)
{
	Magick::Image image = participant.animation.getCurrentImage();

	if (participant.team == Enums::Team::Right)
	{
		image.flop();
	}

	int xDiff = 0;

	if (participant.animation.shaking && participant.animation.status == Enums::Type::Animation::Hit)
	{
		xDiff = sin(RandomUtility::getRandomFloat(0, 1)) * 6 - 3;
	}

	this->drawImage(image, participant.trueSpritePosition + Position(xDiff, 0));
}

void Gif::drawParticipant(const Participant& participant, bool displayNumber)
{
    bool isDead = participant.entity->state->isDead();

    this->drawEntity(participant);

    if (!isDead)
    {
        int effectX = 0;
        int effectY = participant.hudPosition.y;
        int effectSize = 18;

        for (Effect* effect: participant.entity->state->effects)
        {
            Magick::Image effectImage = Assets::getInstance().getEffect(effect);
            Magick::Color effectColor = effect->type == EffectType::Buff ? Magick::Color(77, 146, 191) : Magick::Color(244, 65, 37);
            // Put color on effect
            auto effectPosition = Position(participant.hudPosition.x + (effectX - 1.5) * (effectSize + 2), effectY);
            Magick::Image effectImageColor = Magick::Image(Magick::Geometry(effectSize, effectSize), effectColor);

            effectImage.scale(Magick::Geometry(effectSize, effectSize));
            this->drawImage(effectImageColor, effectPosition);
            this->drawImage(effectImage, effectPosition);

            if (effect->turns > 0)
            {
                this->drawText(Text{
                        .text = std::to_string(effect->turns),
                        .position = Position(participant.hudPosition.x + (effectX - 1.5) * (effectSize + 2) + effectSize / 3, effectY + effectSize / 3),
                        .fontSize = 11
                });
            }
            effectX++;
            if (effectX == 4)
            {
                effectX = 0;
                effectY -= effectSize + 2;
            }
        }
    }

    if (!isDead)
    {
        this->drawHpBar(participant, participant.healthBarPosition);
    }

	if (displayNumber && !isDead)
	{
		Magick::Image attackNumber = Assets::getInstance().getAttackNumber(participant.team, participant.slot);
		int yOffset = 12 + 5;
		int xOffset = -(32 + 5 + 2);

		if (participant.team == Enums::Team::Right)
		{
			xOffset *= -1;
		}

		this->drawImage(attackNumber, Position(participant.healthBarPosition.x + xOffset, participant.healthBarPosition.y + yOffset));
	}

    for (TextInformation textInformation: participant.textInformations)
    {
		Position position = textInformation.offset;

        Text text = Text{
            .text = textInformation.getText(),
            .position = position,
	        .fontSize = textInformation.fontSize,
            .color = textInformation.color
        };

        this->drawText(text);
    }
}

void Gif::drawAttacker(const Participant& attacker)
{
	int y = ((int) Enums::FightConstants::FightHeight) - 20;
	int x = 20 + 16 + 10;

	this->drawText(Text{
		.text = attacker.entity->name + " lvl " + std::to_string(attacker.entity->classType->level),
		.position = Position(x, y),
		.xCentered = false,
		.color = Magick::Color("orange")
	});

	x += 150 + 10;

	this->drawText(Text{
		.text = std::to_string(static_cast<int>(attacker.entity->state->hp)) + "/" + std::to_string(static_cast<int>(attacker.entity->state->maxHp)),
		.position = Position(x, y),
		.xCentered = false
	});

	x += 100 + 10;

	// Draw health bar
	this->drawHpBar(attacker, Position(x, y + 2));
}

void Gif::drawLine(Position start, Position end, int lineWidth, const Magick::Color& color)
{
	this->currentImage.strokeColor(color);
	this->currentImage.strokeWidth(lineWidth);
	this->currentImage.draw(Magick::DrawableLine(start.x, start.y, end.x, end.y));
}

void Gif::drawSkillAnimation(SkillAnimationController animationController)
{
	Position imageOffset = animationController.getImageOffset();

	if (imageOffset == Position(0, 0))
	{
		this->drawImage(animationController.getImage(), animationController.getPosition());
	}
	else
	{
		this->drawImage(animationController.getImage(), animationController.getPosition() + imageOffset, false, false);
	}
}
