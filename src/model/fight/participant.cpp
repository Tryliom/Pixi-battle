#include "model/fight/participant.h"

#include "model/entity/entity.h"
#include "model/entity/fightState.h"
#include "model/entity/classType/classType.h"
#include "model/entity/skill/skill.h"

#include "utility/randomUtility.h"

#include "model/fight/participants.h"


void HealthBarAnimation::updatePastHealth(const Participant& participant)
{
    pastHealth = participant.entity->state->hp;
}

void HealthBarAnimation::calculate(const Participant& participant)
{
	maxHealth = participant.entity->state->maxHp + participant.entity->state->getShield();

	float newHealthPercentage = participant.entity->state->hp / maxHealth;
    float pastHealthPercentage = pastHealth / maxHealth;

    if (!isFinished()) return;

    if (participant.entity->state->hp < pastHealth)
    {
        currentFrame = 0;
        maxFrame = 15;

        difference = pastHealthPercentage - newHealthPercentage;
    }

    currentHealthPercentage = newHealthPercentage;
}

float HealthBarAnimation::getDifferenceOffset(int width) const
{
	float offset = (float) width * currentHealthPercentage;

    return offset;
}

float HealthBarAnimation::getDifferenceWidth() const
{
	auto frame = currentFrame - freezeFrame;

	if (frame < 0)
	{
		frame = 0;
	}

    float percentage = (float) frame / (float) (maxFrame - freezeFrame);

    return difference * (1.f - percentage);
}

void HealthBarAnimation::onFrame()
{
    if (currentFrame < maxFrame)
    {
        currentFrame++;
    }
}

bool HealthBarAnimation::isFinished() const
{
    return currentFrame >= maxFrame;
}

Participant::Participant(Entity* entity, Enums::Team team, Enums::Slot slot) : animation(entity->classType->id, entity->classType->skin)
{
    this->entity = entity;
    this->team = team;
    this->slot = slot;

    this->chosenSkill = nullptr;
    this->targets = {};

    this->autoFight = false;
    this->hide = false;

	this->updatePosition();
}

void Participant::updatePosition()
{
	Magick::Image currentImage = animation.getCurrentImage();
	int participantMaxWidth = static_cast<int>(Enums::FightConstants::ParticipantMaxWidth);
	int participantMaxHeight = static_cast<int>(Enums::FightConstants::ParticipantMaxHeight);

    this->position = Participant::getPosition(this->slot, this->team);

    this->characterPosition = Position(
        this->position.x + participantMaxWidth / 2,
        this->position.y + participantMaxHeight / 2 + 40
    );

	this->trueSpritePosition = Position(
		this->characterPosition.x,
		this->characterPosition.y + static_cast<int>(Enums::FightConstants::CharacterMaxHeight) / 2 - currentImage.size().height() / 2
	);

	this->hudPosition = Position(
		this->characterPosition.x,
		(int) (trueSpritePosition.y - 28 - currentImage.size().height() / 2.0)
	);

	this->healthBarPosition = Position(
		this->hudPosition.x,
		this->hudPosition.y + 12 + 5
	);
}

Position Participant::getPosition(Enums::Slot slot, Enums::Team team)
{
	Position position;
    std::vector<Position> positions = {
        Position(1, 1),
        Position(2, 1),
        Position(1, 2),
        Position(2, 2)
    };
	int participantMaxWidth = static_cast<int>(Enums::FightConstants::ParticipantMaxWidth);
	int participantMaxHeight = static_cast<int>(Enums::FightConstants::ParticipantMaxHeight);

	position = Position(
		(positions[(int) slot].x - 1) * participantMaxWidth,
		(positions[(int) slot].y - 1) * participantMaxHeight
	);

	if (team == Enums::Team::Right) {
		position.x += ((int) Enums::FightConstants::FightWidth - 5) - participantMaxWidth * 2;
	}

    return position;
}

void Participant::initialize(Participants& participants)
{
	this->animation.updateStatus(Enums::Type::Animation::Apparition);

	// Update state with opponents and allies
	for (auto& participant : participants.participants) {
		if (participant.team == this->team) {
			this->entity->state->allies.push_back(participant.entity);
		}
		else {
			this->entity->state->opponents.push_back(participant.entity);
		}
	}

    this->entity->state->displayStatus = [this](TextInformation textInformation)
	{
		for (const TextInformation& information : this->textInformations)
		{
			if (information.text == textInformation.text && information.type == InformationType::Status)
			{
				return;
			}
		}

		Position offset = this->characterPosition;

		int offsetValue = (int) this->animation.getCurrentImage().size().width() / 2;

		if (this->team == Enums::Team::Left && textInformation.type == InformationType::Status
		    || this->team == Enums::Team::Right && textInformation.type == InformationType::HealthDifference)
		{
			offset.x += offsetValue;
		}
		else
		{
			offset.x -= offsetValue;
		}

		std::vector listOfPositions = textInformation.type == InformationType::Status ? STATUS_POSITIONS : HEALTH_POSITIONS;

		for (Position spot : listOfPositions)
		{
			bool found = true;

			for (const TextInformation& otherInformation : this->textInformations)
			{
				if (otherInformation.offset == (spot + offset))
				{
					found = false;
					break;
				}
			}

			if (found)
			{
				offset += spot;
				break;
			}
		}

		textInformation.offset = offset;

        this->textInformations.push_back(textInformation);
    };
}

void Participant::onFrame()
{
	animation.nextFrame();

	if (animation.status != Enums::Type::Animation::Death)
	{
		if (animation.isFinished() && animation.status != Enums::Type::Animation::Hit)
		{
			animation.updateStatus(Enums::Type::Animation::Idle);
		}

		if (entity->state->isDead())
		{
			animation.updateStatus(Enums::Type::Animation::Death);
		}
	}

    healthBarAnimation.onFrame();
    healthBarAnimation.updatePastHealth(*this);

    // Remove text information if finished
    for (auto it = textInformations.begin(); it != textInformations.end();)
    {
        if (it->isFinished())
        {
            it = textInformations.erase(it);
        }
        else
        {
            it++;
        }
    }

    for (auto& textInformation : textInformations)
    {
        textInformation.nextFrame();
    }
}

Enums::Type::Line Participant::getLine() const
{
	if (this->team == Enums::Team::Left && (this->slot == Enums::Slot::TopLeft || this->slot == Enums::Slot::BottomLeft) ||
	    this->team == Enums::Team::Right && (this->slot == Enums::Slot::TopRight || this->slot == Enums::Slot::BottomRight))
	{
		return Enums::Type::Line::Back;
	}

	return Enums::Type::Line::Front;
}

void Participant::resetAction()
{
	this->chosenSkill = nullptr;
	this->targets = {};
}

void Participant::selectSkill(ActiveSkill* skill)
{
	this->chosenSkill = skill;
}

void Participant::selectTarget(const std::vector<ParticipantTarget>& target)
{
	this->targets = target;
}

std::vector<std::vector<ParticipantTarget>> Participant::getAllPossibleTargets(ActiveSkill* skill, Participants& participants)
{
	std::vector<std::vector<ParticipantTarget>> possibleTargets;

	if (skill->targetType == Enums::Type::Target::Self)
	{
		possibleTargets.push_back({ { this->slot, this->team} });

		return possibleTargets;
	}
	else if (skill->targetType == Enums::Type::Target::SingleEmptyAlly || skill->targetType == Enums::Type::Target::SingleEmptyEnemy)
	{
		// Get the list of empty allies/enemies cells, where there is no participant
		std::vector<Enums::Slot> emptySlots;
		Enums::Team emptyTeam = skill->targetType == Enums::Type::Target::SingleEmptyAlly ? this->team : this->team == Enums::Team::Left ? Enums::Team::Right : Enums::Team::Left;

		for (Enums::Slot emptySlot : {Enums::Slot::BottomLeft, Enums::Slot::BottomRight, Enums::Slot::TopLeft, Enums::Slot::TopRight})
		{
			bool found = false;

			for (Participant& participant : participants.participants)
			{
				if (participant.slot == emptySlot && participant.team == emptyTeam)
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				emptySlots.push_back(emptySlot);
			}
		}

		for (Enums::Slot emptySlot : emptySlots)
		{
			possibleTargets.push_back({ {emptySlot, emptyTeam} });
		}

		return possibleTargets;
	}

	for (Participant& participant : participants.participants)
	{
		if (
			skill->targetType == Enums::Type::Target::SingleEnemy && participant.team != this->team && !participant.isDead()
			|| skill->targetType == Enums::Type::Target::SingleAlly && participant.team == this->team && !participant.isDead()
			|| skill->targetType == Enums::Type::Target::SingleDeadAlly && participant.team == this->team && participant.isDead()
			|| skill->targetType == Enums::Type::Target::SingleDeadEnemy && participant.team != this->team && participant.isDead()
		)
		{
			possibleTargets.push_back({ { participant.slot, participant.team} });
		}
		else if (
			skill->targetType == Enums::Type::Target::AllAllies && participant.team == this->team && !participant.isDead()
			|| skill->targetType == Enums::Type::Target::AllEnemies && participant.team != this->team && !participant.isDead()
			|| skill->targetType == Enums::Type::Target::AllDeadAllies && participant.team == this->team && participant.isDead()
			|| skill->targetType == Enums::Type::Target::AllDeadEnemies && participant.team != this->team && participant.isDead()
		)
		{
			if (possibleTargets.empty())
			{
				possibleTargets.emplace_back();
			}

			possibleTargets[0].emplace_back(ParticipantTarget{participant.slot, participant.team});
		}
	}

	return possibleTargets;
}

bool Participant::isDead() const
{
	return this->entity->state->isDead();
}

bool Participant::needToSelectSkill() const
{
	return this->chosenSkill == nullptr;
}

bool Participant::needToSelectTarget() const
{
	return this->targets.empty();
}

void Participant::useSkill(Participants& participants)
{
	if (this->chosenSkill == nullptr)
	{
		return;
	}

	if (this->targets.empty())
	{
		std::vector<std::vector<ParticipantTarget>> possibleTargets = this->getAllPossibleTargets(this->chosenSkill, participants);

		if (!possibleTargets.empty())
		{
			this->targets = possibleTargets[0];
		}
	}

	if (this->targets.empty())
	{
		return;
	}

	this->chosenSkill->use(participants, *this, this->targets);

	this->chosenSkill = nullptr;
	this->targets = {};
}
