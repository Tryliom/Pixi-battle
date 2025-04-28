#pragma once

#include "model/fight/participant.h"
#include "model/fight/participants.h"

struct Ai
{
	void playTurn(Participant* participant, Participants* participants);
	std::vector<Participant*> getTargets(Skill* skill, Participant* participant, Participants* participants);
	std::vector<Skill*> getUsableSkills(Participant* target, Participant* participant, Participants* participants);
	bool canFindTarget(Skill* skill, Participant* participant, Participants* participants);
};
