#include "model/entity/skill/skillAnimation.h"

#include "model/fight/participant.h"

#include "model/entity/skill/skill.h"
#include "model/entity/entity.h"

SkillAnimation::SkillAnimation(const std::string& name, const std::vector<AnimationStep>& steps, SkillTargetType targetType)
{
	this->name = name;
	this->steps = steps;
	this->targetType = targetType;
}

std::string AnimationStep::getName() const
{
	switch (this->type)
	{
	case SkillAnimationType::Projectile:
		return "projectile";
	case SkillAnimationType::Impact:
		return "impact";
	case SkillAnimationType::Return:
		return "return";
	default:
		return "unknown";
	}
}

SkillAnimationController::SkillAnimationController(ActiveSkill* skill, Participant& attacker, const std::vector<ParticipantTarget>& targets, Participants& participants) :
	attacker(attacker), targets(targets), participants(participants)
{
	this->skill = skill;

	calculateAnimation();
}

void SkillAnimationController::useSkill()
{
	this->skill->use(participants, this->attacker, targets);
	this->attacker.entity->onEndTurn(this->skill);
}

float SkillAnimationController::getPercentage() const
{
	return (float) this->travelTime / (float) this->maxTravelTime;
}

bool SkillAnimationController::isFinished() const
{
	return this->currentStep == this->skill->animation.steps.size() - 1 && this->currentFrame == this->currentAnimation.size() - 1;
}

void SkillAnimationController::nextStep()
{
	this->currentStep++;
	this->currentFrame = 0;

	calculateAnimation();
}

void SkillAnimationController::calculateAnimation()
{
	AnimationStep step = this->skill->animation.steps[this->currentStep];
	this->currentAnimation = Assets::getInstance().getSkillAnimation(this->skill->animation.name + "_" + step.getName());

	if (step.type == SkillAnimationType::Projectile)
	{
		this->start = getAttackerPosition();
		this->end = getTargetPosition();
		this->maxTravelTime = step.travelTime;
	}
	else if (step.type == SkillAnimationType::Impact)
	{
		this->start = getTargetPosition();
		this->end = this->start;
		this->maxTravelTime = 0;
	}
	else if (step.type == SkillAnimationType::Return)
	{
		this->start = getTargetPosition();
		this->end = getAttackerPosition();
		this->maxTravelTime = step.travelTime;
	}
}


Position SkillAnimationController::getAttackerPosition()
{
	AnimationStep step = this->skill->animation.steps[this->currentStep];

	return this->attacker.trueSpritePosition + step.attackerOffset * static_cast<int>(this->attacker.team);
}

Position SkillAnimationController::getTargetPosition()
{
	//todo: check if target is an empty cell
	Position trueSpritePosition;

	if (this->skill->targetType == Enums::Type::Target::SingleEmptyEnemy || this->skill->targetType == Enums::Type::Target::SingleEmptyAlly)
	{
		trueSpritePosition = Participant::getPosition(this->targets[0].slot, this->targets[0].team);
	}
	else
	{
		trueSpritePosition = participants.getParticipant(this->targets[0].team, this->targets[0].slot).trueSpritePosition;
	}

	Position targetPosition = trueSpritePosition;
	AnimationStep step = this->skill->animation.steps[this->currentStep];

	if (this->skill->animation.targetType == SkillTargetType::Single)
	{
		targetPosition = trueSpritePosition + step.targetOffset * static_cast<int>(this->targets[0].team);
	}
	else if (this->skill->animation.targetType == SkillTargetType::All)
	{
		// Get the center of all targets
		Position total = Position(0, 0);

		for (ParticipantTarget& target : this->targets)
		{
			total += participants.getParticipant(this->targets[0].team, this->targets[0].slot).trueSpritePosition;
		}

		targetPosition = total / static_cast<int>(this->targets.size()) + step.targetOffset * static_cast<int>(this->targets[0].team);
	}
	else if (this->skill->animation.targetType == SkillTargetType::Line)
	{
		// Get the end of the line where the targets are according where the attacker is
		if (attacker.trueSpritePosition.x < trueSpritePosition.x)
		{
			targetPosition = Position(static_cast<int>(Enums::FightConstants::FightWidth), trueSpritePosition.y);
		}
		else
		{
			targetPosition = Position(0, trueSpritePosition.y);
		}
	}

	return targetPosition;
}

void SkillAnimationController::onFrame()
{
	AnimationStep step = this->skill->animation.steps[this->currentStep];

	if (step.useSkillFrame == this->currentFrame)
	{
		useSkill();
	}

	if (this->currentFrame == this->currentAnimation.size() - 1 && step.type == SkillAnimationType::Impact
		|| this->travelTime == this->maxTravelTime && this->maxTravelTime != 0)
	{
		nextStep();
	}
	else if (this->currentFrame == this->currentAnimation.size() - 1 && step.type != SkillAnimationType::Impact)
	{
		this->currentFrame = 0;
	}

	this->currentFrame++;

	if (this->maxTravelTime != 0)
	{
		this->travelTime++;
	}

	if (isFinished())
	{
		if (step.useSkillFrame == -1)
		{
			useSkill();
		}
	}
}

Position SkillAnimationController::getPosition()
{
	AnimationStep step = this->skill->animation.steps[this->currentStep];
	Position position = this->start;

	if (step.type != SkillAnimationType::Impact)
	{
		position += (this->end - this->start) * getPercentage();
	}

	return position;
}

Magick::Image SkillAnimationController::getImage()
{
	Magick::Image image = this->currentAnimation[this->currentFrame];
	AnimationStep step = this->skill->animation.steps[this->currentStep];
	Position attackerPosition = getAttackerPosition();
	Position targetPosition = getTargetPosition();

	if (this->start.x > this->end.x)
	{
		image.flip();
	}

	// Rotate the image to face the target
	if (attackerPosition.x != targetPosition.x && step.rotateWithAngle)
	{
		double angle = atan2(targetPosition.y - attackerPosition.y, targetPosition.x - attackerPosition.x) * 180 / M_PI;

		image.backgroundColor(Magick::Color("transparent"));
		image.rotate(angle);
	}

	return image;
}

Position SkillAnimationController::getImageOffset()
{
	AnimationStep step = this->skill->animation.steps[this->currentStep];
	Magick::Geometry imageSize = this->currentAnimation[this->currentFrame].size();
	int imageWidth = static_cast<int>(imageSize.width());
	int imageHeight = static_cast<int>(imageSize.height());
	Magick::Geometry geometry;

	if (this->skill->targetType == Enums::Type::Target::SingleEmptyEnemy || this->skill->targetType == Enums::Type::Target::SingleEmptyAlly)
	{
		geometry = Magick::Geometry(64, 64);
	}
	else
	{
		geometry = participants.getParticipant(this->targets[0].team, this->targets[0].slot).animation.getCurrentImage().size();
	}

	int characterWidth = static_cast<int>(geometry.width());
	int characterHeight = static_cast<int>(geometry.height());

	// The image position is the top left corner of the image from the center of character
	switch (step.targetAnchor)
	{
		case SkillAnimationAnchor::Top:
			return {-imageWidth / 2, -characterHeight / 2};
		case SkillAnimationAnchor::TopLeft:
			return {-characterWidth, -characterHeight / 2};
		case SkillAnimationAnchor::TopRight:
			return {characterWidth / 2 - imageWidth, -characterHeight / 2};
		case SkillAnimationAnchor::Bottom:
			return {-imageWidth / 2, characterHeight / 2 - imageHeight};
		case SkillAnimationAnchor::BottomLeft:
			return {-characterWidth, characterHeight / 2 - imageHeight};
		case SkillAnimationAnchor::BottomRight:
			return {characterWidth / 2 - imageWidth, characterHeight / 2 - imageHeight};
		case SkillAnimationAnchor::Left:
			return {-characterWidth, -imageHeight / 2};
		case SkillAnimationAnchor::Right:
			return {characterWidth / 2 - imageWidth, -imageHeight / 2};
		default:
			return {0, 0};
	}
}
