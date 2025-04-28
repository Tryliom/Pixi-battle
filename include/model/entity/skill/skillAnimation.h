#pragma once

#include <string>
#include <Magick++/Image.h>

#include "model/fight/position.h"

struct ActiveSkill;
struct Participant;
struct Participants;
struct ParticipantTarget;

enum class SkillAnimationType
{
	Projectile,
	Impact,
	Return
};

enum class SkillTargetType
{
	Single,
	All,
	Line
};

enum class SkillAnimationAnchor
{
	Top, Bottom, Left, Right, Center,
	TopLeft, TopRight, BottomLeft, BottomRight
};

struct AnimationStep
{
	SkillAnimationType type { SkillAnimationType::Projectile };

	// Apply only to single target
	Position attackerOffset { Position(0, 0) };
	Position targetOffset { Position(0, 0) };

	// Apply only to the image
	SkillAnimationAnchor targetAnchor { SkillAnimationAnchor::Center };

	// Apply only to single target
	bool rotateWithAngle { true };

	// Time in frame, 20 frames per second
	int travelTime { 10 };
	int useSkillFrame { -1 };

	[[nodiscard]] std::string getName() const;
};

struct SkillAnimation
{
	SkillAnimation() = default;
	SkillAnimation(const std::string& name, const std::vector<AnimationStep>& steps, SkillTargetType targetType);

	std::string name;
	std::vector<AnimationStep> steps;
	SkillTargetType targetType { SkillTargetType::Single };
};

struct SkillAnimationController
{
	SkillAnimationController(ActiveSkill* skill, Participant& attacker, const std::vector<ParticipantTarget>& targets, Participants& participants);

private:
	ActiveSkill* skill;
	Participant& attacker;
	std::vector<ParticipantTarget> targets;
	Participants& participants;

	int currentStep = 0;

	std::vector<Magick::Image> currentAnimation;
	int currentFrame = 0;

	Position start;
	Position end;

	// Time in frame, 20 frames per second
	int maxTravelTime { 20 };
	int travelTime { 0 };

	[[nodiscard]] float getPercentage() const;

	void nextStep();
	/**
	 * Called when go to next step
	 */
	void calculateAnimation();
	void useSkill();

	Position getAttackerPosition();
	Position getTargetPosition();

public:
	[[nodiscard]] bool isFinished() const;

	void onFrame();

	Position getPosition();
	Magick::Image getImage();
	Position getImageOffset();
};