#pragma once

#include "model/image/animation.h"
#include "model/fight/position.h"
#include "model/entity/textInformation.h"

struct Entity;
struct Skill;
struct ActiveSkill;
struct Participants;
struct Participant;

const std::vector<Position> HEALTH_POSITIONS = { Position(0, 0), Position(0, -22), Position(0, 22), Position(0, -44), Position(0, 44) };
const std::vector<Position> STATUS_POSITIONS = { Position(0, 0), Position(0, -17), Position(0, 17), Position(0, -34), Position(0, 34) };

struct HealthBarAnimation
{
    int currentFrame { 10 };
    int maxFrame { 15 };
	int freezeFrame { 5 };

	// The max health, with shield
	float maxHealth { 0.f };
    float currentHealthPercentage { 0.f };

    float pastHealth { 0.f };
    float difference { 0.f };

    void updatePastHealth(const Participant& participant);
    void calculate(const Participant& participant);
    [[nodiscard]] float getDifferenceOffset(int width) const;
    [[nodiscard]] float getDifferenceWidth() const;

    void onFrame();
    [[nodiscard]] bool isFinished() const;
};

struct ParticipantTarget
{
	Enums::Slot slot { Enums::Slot::Unknown };
	Enums::Team team { Enums::Team::Unknown };
};

struct Participant
{
    Participant(Entity* entity, Enums::Team team, Enums::Slot slot);

    Entity* entity;
    Enums::Team team;
    Enums::Slot slot;
    Animation animation;

    HealthBarAnimation healthBarAnimation;
    std::vector<TextInformation> textInformations;

    dpp::timer timer{};
	ActiveSkill* chosenSkill;
	std::vector<ParticipantTarget> targets;
    /**
     * The position is top left corner of the participant.
     */
    Position position{ Position(0, 0) };
    /**
     * The position is the middle of the 64x64 character space.
     */
    Position characterPosition{ Position(0, 0) };
	/**
	 * The position is the middle of the sprite.
	 */
	Position trueSpritePosition{ Position(0, 0) };
	/**
	 * The position bottom middle of the hud (start of the effects)
	 */
	Position hudPosition{ Position(0, 0) };
	/**
	 * The position is the middle of the health bar.
	 */
	Position healthBarPosition{ Position(0, 0) };

    bool autoFight;
    bool hide;

	//Temp
	bool hasAttack;

    void initialize(Participants& participants);
    void onFrame();

    void updatePosition();
	[[nodiscard]] Enums::Type::Line getLine() const;
	[[nodiscard]] bool isDead() const;

    /*void checkAutoFight();
    void toggleAutoFight();*/
    /*void playTurn();*/

    void resetAction();
	void selectSkill(ActiveSkill* skill);
	void selectTarget(const std::vector<ParticipantTarget>& target);

	void useSkill(Participants& participants);
    /*void counterAttack(Skill* skill, std::vector<Participant&> targets);

     void onEndTurn();*/

    std::vector<std::vector<ParticipantTarget>> getAllPossibleTargets(ActiveSkill* skill, Participants& participants);

	/*bool hasChooseAttack();*/
    bool needToSelectSkill() const;
    bool needToSelectTarget() const;

	static Position getPosition(Enums::Slot slot, Enums::Team team);
    //TODO: Add useSkillAnimation()
};
