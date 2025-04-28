#include "model/fight/participants.h"

#include "model/entity/entity.h"
#include "model/entity/fightState.h"
#include "model/entity/classType/classType.h"
#include "model/entity/skill/skill.h"
#include "manager/iconManager.h"

void TurnOrderAnimation::increaseFrame()
{
	currentFrame++;
}

bool TurnOrderAnimation::isFinished() const
{
	return currentFrame >= maxFrame || turnOrderPositions.empty();
}

float TurnOrderAnimation::getPercentage() const
{
	return (float) currentFrame / (float) maxFrame;
}

TransitionOrder::TransitionOrder(Participant& participant, int indexA, int indexB)
{
	team = participant.team;
	classID = participant.entity->classType->id;

	transition = Transition::MOVE;

	if (indexA == -1)
	{
		indexA = indexB;
	}

	this->indexA = indexA;
	this->indexB = indexB;
}

Participants::Participants(const std::vector<Participant>& participants)
{
	this->participants = participants;
}

std::vector<ParticipantOrder> Participants::getDisplayOrder()
{
    std::vector<ParticipantDisplayOrder> order = {
        { Enums::Team::Left, Enums::Slot::TopLeft },
        { Enums::Team::Left, Enums::Slot::TopRight },
        { Enums::Team::Right, Enums::Slot::TopLeft },
        { Enums::Team::Right, Enums::Slot::TopRight },
        { Enums::Team::Left, Enums::Slot::BottomLeft },
        { Enums::Team::Left, Enums::Slot::BottomRight },
        { Enums::Team::Right, Enums::Slot::BottomLeft },
        { Enums::Team::Right, Enums::Slot::BottomRight }
    };

    std::vector<ParticipantOrder> displayOrder;

    for (int i = 0; i < order.size(); i++)
    {
        ParticipantDisplayOrder& participantDisplayOrder = order[i];
        bool found = false;

        for (int j = 0; j < participants.size(); j++)
        {
            Participant& participant = participants[j];

            if (participant.team == participantDisplayOrder.team && participant.slot == participantDisplayOrder.slot)
            {
                displayOrder.push_back({ j, false, participantDisplayOrder });
                found = true;
                break;
            }
        }

        if (!found)
        {
            displayOrder.push_back({ -1, false, participantDisplayOrder });
        }

        if (i == 3)
        {
            displayOrder.push_back({ -1, true });
        }
    }

    return displayOrder;
}

Participant& Participants::getParticipant(int index)
{
	return participants[index];
}

void Participants::addParticipant(const Participant& participant)
{
    participants.push_back(participant);
}

void Participants::removeParticipant(int index)
{
    participants.erase(participants.begin() + index);
}

void Participants::moveParticipant(ParticipantDisplayOrder previous, ParticipantDisplayOrder newOrder)
{
	for (int i = 0; i < participants.size(); i++)
	{
		Participant& participant = participants[i];

		if (participant.team == previous.team && participant.slot == previous.slot)
		{
			participant.team = newOrder.team;
			participant.slot = newOrder.slot;
			break;
		}
	}
}

std::vector<int> Participants::getParticipantsTurnOrder(bool keepAttackerFirst)
{
	int maxLength = 10;
	std::vector<TempParticipantTurnOrder> tempParticipants;

	this->updateMaxSpeed();

	for (int i = 0; i < participants.size(); i++)
	{
		Participant& participant = participants[i];

		if (participant.entity->state->isDead())
		{
			continue;
		}

		float speed = participant.entity->stats.speed;
		float attackBarPercentage = participant.entity->state->attackBarPercent;

		tempParticipants.push_back(
			{
				i,
				speed,
				attackBarPercentage
			}
		);
	}

	std::vector<int> participantsTurnOrderIndex;

	if (keepAttackerFirst && static_cast<int>(currentTurnOrder.size()) > 0)
	{
		participantsTurnOrderIndex.push_back(this->currentTurnOrder[0]);

		for (int i = 0; i < tempParticipants.size(); i++)
		{
			if (tempParticipants[i].index == this->currentTurnOrder[0])
			{
				tempParticipants[i].attackBarPercentage -= 1.f;
			}
		}
	}

	for (int i = 0; i < maxLength; i++)
	{
		float fasterParticipantTime = 1.f;
		int fasterParticipantIndex = 0;

		for (int j = 0; j < tempParticipants.size(); j++)
		{
			float participantTime = this->getAttackBarFactor(tempParticipants[j].attackBarPercentage, tempParticipants[j].speed) / 100.f;

			if (j == 0 || participantTime < fasterParticipantTime)
			{
				fasterParticipantTime = participantTime;
				fasterParticipantIndex = j;
			}
		}

		participantsTurnOrderIndex.push_back(tempParticipants[fasterParticipantIndex].index);

		for (int j = 0; j < tempParticipants.size(); j++)
		{
			tempParticipants[j].attackBarPercentage += fasterParticipantTime / (maxSpeed / tempParticipants[j].speed) / 100.f;
		}

		tempParticipants[fasterParticipantIndex].attackBarPercentage -= 1.f;
	}

	return participantsTurnOrderIndex;
}

bool Participants::isEveryoneDead()
{
	for (Participant& participant: participants)
	{
		if (!participant.entity->state->isDead())
		{
			return false;
		}
	}

	return true;
}

void Participants::setNewTurnOrder(const std::vector<int>& newTurnOrder, bool keepAttackerFirst)
{
	// Check if there is any change in the turn order
	if (newTurnOrder == currentTurnOrder)
	{
		return;
	}

	if (!currentTurnOrder.empty())
	{
		// Get all changed indexes
		std::vector<TransitionOrder> turnOrderPositions;
		std::vector<int> oldIndexUsed = {0};

		for (int i = 0; i < newTurnOrder.size(); i++)
		{
			if (newTurnOrder[i] != currentTurnOrder[i])
			{
				int participantIndex = newTurnOrder[i];
				int oldTurnOrderIndex = -1;
				Participant& participant = participants[participantIndex];

				// Find the old index of the participant
				for (int j = 0; j < currentTurnOrder.size(); j++)
				{
					if ((j != 0 || keepAttackerFirst) && currentTurnOrder[j] == participantIndex && std::find(oldIndexUsed.begin(), oldIndexUsed.end(), j) == oldIndexUsed.end())
					{
						oldTurnOrderIndex = j;
						break;
					}
				}

				turnOrderPositions.emplace_back(participant, oldTurnOrderIndex, i);

				if (oldTurnOrderIndex != -1) {
					oldIndexUsed.emplace_back(oldTurnOrderIndex);
				}
			}
			else if (i != 0 || keepAttackerFirst)
			{
				turnOrderPositions.emplace_back(participants[newTurnOrder[i]], i, i);
				oldIndexUsed.emplace_back(i);
			}
		}

		// Sort turn order positions by transition type
		std::sort(turnOrderPositions.begin(), turnOrderPositions.end(), [](const TransitionOrder& a, const TransitionOrder& b) {
			return a.transition < b.transition;
		});

		// Update the turn order
		this->turnOrderAnimation = TurnOrderAnimation{turnOrderPositions};
	}

	this->currentTurnOrder = newTurnOrder;
}

void Participants::updateTurnOrder()
{
	setNewTurnOrder(getParticipantsTurnOrder(true), true);
}

void Participants::nextTurn()
{
	if (isFightFinished()) return;

	setNewTurnOrder(getParticipantsTurnOrder(false), false);

	Participant& attacker = participants[currentTurnOrder[0]];
	float attackBarFactor = getAttackBarFactor(attacker.entity->state->attackBarPercent, attacker.entity->stats.speed);

	for (Participant& participant: participants)
	{
		if (participant.animation.status == Enums::Type::Animation::Hit)
		{
			participant.animation.status = Enums::Type::Animation::Idle;
		}

		participant.entity->state->attackBarPercent += attackBarFactor / (maxSpeed / participant.entity->stats.speed);
	}
}

void Participants::updateMaxSpeed()
{
	maxSpeed = 0;

	for (Participant& participant: participants)
	{
		if (participant.entity->stats.speed > maxSpeed)
		{
			maxSpeed = participant.entity->stats.speed;
		}
	}
}

float Participants::getAttackBarFactor(float attackBarPercentage, float speed) const
{
	float attackBarToGet = 1.f - attackBarPercentage;

	if (attackBarToGet < 0)
	{
		attackBarToGet = 0;
	}

	return attackBarToGet * (maxSpeed / speed);
}

void Participants::initialize()
{
	hasFightStarted = true;

	for (Participant& participant: participants)
	{
		participant.initialize(*this);
		participant.entity->onStartFight(*this, participant.getLine());
	}

	nextTurn();
}

void Participants::onFrame(Gif& gif)
{
	for (Participant& participant: participants)
	{
		gif.drawParticipant(participant, this->playerMustPlay() && this->isAllAnimationsFinished());

		participant.onFrame();

		participant.entity->updateEffects();
		participant.entity->updateStats();
		participant.entity->updateSkills();
	}

	Participant& attacker = participants[currentTurnOrder[0]];

	gif.drawAttackBar(*this);
	gif.drawAttacker(attacker);

	// Check if skillAnimations are done, if yes, clear them and start the next turn
	if (!skillAnimations.empty() && turnOrderAnimation.isFinished())
	{
		bool skillAnimationsDone = true;

		for (SkillAnimationController& skillAnimation: skillAnimations)
		{
			skillAnimation.onFrame();

			if (!skillAnimation.isFinished())
			{
				gif.drawSkillAnimation(skillAnimation);
				skillAnimationsDone = false;
			}
		}

		if (skillAnimationsDone)
		{
			skillAnimations.clear();
			nextTurn();
		}
	}

	if (turnOrderAnimation.isFinished() && !isFightFinished() && isAllAnimationsFinished())
	{
		if (!attacker.entity->state->turnActive)
		{
			//TODO: Run events on turn start
			attacker.entity->onStartTurn();
			attacker.hasAttack = false;

			updateTurnOrder();
		}
		else if (attacker.entity->type == Enums::Type::Entity::Monster
			|| (attacker.entity->type == Enums::Type::Entity::Player && !attacker.needToSelectTarget() && !attacker.needToSelectSkill()))
		{
			if (attacker.entity->state->turnActive && !attacker.hasAttack)
			{
				attacker.animation.updateStatus(Enums::Type::Animation::Attack);
				attacker.hasAttack = true;
			}

			if (attacker.entity->state->turnActive && attacker.animation.status == Enums::Type::Animation::Idle && skillAnimations.empty())
			{
				//TODO: Check the skill target type and add new SkillAnimationControllers to the list
				ActiveSkill* activeSkill = attacker.chosenSkill;
				std::vector<ParticipantTarget> targets = attacker.targets;

				if (activeSkill == nullptr)
				{
					activeSkill = attacker.entity->activeSkills[0];
				}

				if (targets.empty())
				{
					targets = attacker.getAllPossibleTargets(activeSkill, *this)[0];
				}

				if (activeSkill->animation.targetType == SkillTargetType::Single)
				{
					//TODO: Loop into the targets choose by the skill and add a SkillAnimationControllers for each of them
				}
				else if (activeSkill->animation.targetType == SkillTargetType::All || activeSkill->animation.targetType == SkillTargetType::Line)
				{
					//TODO: Add one SkillAnimationControllers for all the targets
				}

				skillAnimations.emplace_back(activeSkill, attacker, targets, *this);
			}
		}
	}

	for (Participant& participant: participants)
    {
        participant.healthBarAnimation.calculate(participant);
    }

	// Animations
	turnOrderAnimation.increaseFrame();
}

bool Participants::isAnimationsFinished() const
{
	for (const Participant& participant: participants)
	{
		if (!participant.healthBarAnimation.isFinished()) return false;
	}

	return skillAnimations.empty();
}

bool Participants::isAllAnimationsFinished() const
{
	for (const Participant& participant: participants)
	{
		if ((participant.animation.status != Enums::Type::Animation::Idle
			&& participant.animation.status != Enums::Type::Animation::Death
			&& !participant.animation.isFinished()) || !participant.textInformations.empty()) return false;
	}

	return isAnimationsFinished() && turnOrderAnimation.isFinished();
}

bool Participants::isFightFinished()
{
	int leftTeamCount = 0;
	int rightTeamCount = 0;

	for (Participant& participant: participants)
	{
		if (participant.team == Enums::Team::Left)
		{
			if (!participant.entity->state->isDead())
			{
				leftTeamCount++;
			}
		}
		else if (participant.team == Enums::Team::Right)
		{
			if (!participant.entity->state->isDead())
			{
				rightTeamCount++;
			}
		}
	}

	return (leftTeamCount == 0 || rightTeamCount == 0) && isAnimationsFinished();
}

bool Participants::playerMustPlay()
{
	Participant& participant = participants[currentTurnOrder[0]];

	return participant.entity->type == Enums::Type::Entity::Player &&
		participant.entity->state->turnActive && (participant.chosenSkill == nullptr || participant.targets.empty());
}

int Participants::countByFilter(const Filter& filter)
{
	int count = 0;

	for (Participant& participant: participants)
	{
		if (filter(participant))
		{
			count++;
		}
	}

	return count;
}

Participant& Participants::getCurrentTurnParticipant()
{
	return participants[currentTurnOrder[0]];
}

Participant& Participants::getParticipant(Enums::Team team, Enums::Slot slot)
{
	for (Participant& participant: participants)
	{
		if (participant.team == team && participant.slot == slot)
		{
			return participant;
		}
	}

	throw std::runtime_error("No participant found for team " + std::to_string(static_cast<int>(team)) + " and slot " + std::to_string(static_cast<int>(slot)));
}

void Participants::addParticipantsButtons(std::vector<dpp::component>& components, const std::function<void(dpp::component&, ParticipantOrder)>& onButtonCreation)
{
	auto participantDisplayOrder = getDisplayOrder();
	dpp::component row;
	row.set_type(dpp::cot_action_row);
	int i = 0;

	for (auto& participantOrder : participantDisplayOrder)
	{
		dpp::component button;
		button.set_id("slot-" + std::to_string(i));
		button.set_type(dpp::cot_button);

		row.set_type(dpp::cot_action_row);

		if (participantOrder.index == -1)
		{
			button.style = dpp::cos_primary;
		}
		else
		{
			Participant& participant = getParticipant(participantOrder.index);

			dpp::emoji* emoji = IconManager::getEmoji(participant.entity->classType->emojiName);

			if (emoji == nullptr)
			{
				emoji = IconManager::getEmoji("default");
			}

			button.set_emoji(emoji->name, emoji->id);
			button.style = dpp::cos_secondary;
		}

		if (!participantOrder.newLine)
		{
			onButtonCreation(button, participantOrder);

			row.add_component(button);
		}

		if (row.components.size() == 2)
		{
			// Place a separator button disabled
			dpp::component separator;
			separator.set_type(dpp::cot_button);
			separator.style = dpp::cos_secondary;
			separator.set_emoji("✖️");
			separator.set_id("separator-" + std::to_string(i));
			separator.set_disabled(true);

			row.add_component(separator);
		}

		if (row.components.size() == 5)
		{
			components.emplace_back(row);
			row = dpp::component();
		}

		i++;
	}
}

bool Filter::operator()(const Participant& participant) const
{
	if (team != Enums::Team::Unknown && participant.team != team) return false;
	if (slot != Enums::Slot::Unknown && participant.slot != slot) return false;
	if (type != Enums::Type::Entity::Unknown && participant.entity->type != type) return false;
	if (dead != -1 && !(participant.entity->state->isDead() && dead == 1)) return false;

	return true;
}
