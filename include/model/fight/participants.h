#pragma once

#include "model/fight/participant.h"
#include "model/image/gif.h"
#include "model/entity/skill/skillAnimation.h"

struct TempParticipantTurnOrder
{
	int index;
	float speed;
	float attackBarPercentage;
};

struct TransitionOrder
{
	TransitionOrder(Participant& participant, int indexA, int indexB);

	Enums::Team team;
	std::string classID;

	int indexA;
	int indexB;

	Transition transition;
};

struct TurnOrderAnimation {
	// Animations from point A to point B
	std::vector<TransitionOrder> turnOrderPositions {};

	int currentFrame { 0 };
	int maxFrame { 10 };

	[[nodiscard]] bool isFinished() const;
	[[nodiscard]] float getPercentage() const;
	void increaseFrame();
};

struct ParticipantDisplayOrder
{
    Enums::Team team { Enums::Team::Unknown };
    Enums::Slot slot { Enums::Slot::Unknown };
};

struct ParticipantOrder {
    // -1 if empty
    int index { -1 };
    bool newLine { false };
	ParticipantDisplayOrder displayOrder;
};

struct Filter {
	Enums::Team team { Enums::Team::Unknown };
	Enums::Slot slot { Enums::Slot::Unknown };
	Enums::Type::Entity type { Enums::Type::Entity::Unknown };

	Enums::Type::Line line { Enums::Type::Line::Unknown };
	/**
	 * -1 if no filter, 0 if alive, 1 if dead
	 */
	int dead { -1 };

	bool operator()(const Participant& participant) const;
};

struct Participants final
{
    explicit Participants(const std::vector<Participant>& participants);

    // Always sort it in slot + team order
    std::vector<Participant> participants;
    std::vector<int> currentTurnOrder;
    float maxSpeed { 0.f };

	// Animations
	TurnOrderAnimation turnOrderAnimation;
	std::vector<SkillAnimationController> skillAnimations;

	bool hasFightStarted { false };

    std::vector<ParticipantOrder> getDisplayOrder();

	Participant& getParticipant(int index);
	Participant& getParticipant(Enums::Team team, Enums::Slot slot);
	Participant& getCurrentTurnParticipant();

    void addParticipant(const Participant& participant);
    void removeParticipant(int index);
	void moveParticipant(ParticipantDisplayOrder previous, ParticipantDisplayOrder newOrder);

	std::vector<int> getParticipantsTurnOrder(bool keepAttackerFirst);
	void setNewTurnOrder(const std::vector<int>& newTurnOrder, bool keepAttackerFirst);
	void updateTurnOrder();

    void updateMaxSpeed();
    [[nodiscard]] float getAttackBarFactor(float attackBarPercentage, float speed) const;

	void initialize();
	void onFrame(Gif& gif);
    void nextTurn();

    bool isEveryoneDead();
	bool isFightFinished();
	bool playerMustPlay();

	int countByFilter(const Filter& filter);

	[[nodiscard]] bool isAnimationsFinished() const;
	[[nodiscard]] bool isAllAnimationsFinished() const;

	// Each participant button has the id "slot-<index>"
	void addParticipantsButtons(std::vector<dpp::component>& components, const std::function<void(dpp::component&, ParticipantOrder)>& onButtonCreation = [](dpp::component&, ParticipantOrder) {});
};
