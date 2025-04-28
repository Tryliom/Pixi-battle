#pragma once

#include "utility/commandInterface.h"
#include "model/fight/participants.h"

struct Participant;

// Allow only players to join left team
struct LobbyInterface : public CommandInterface
{
    explicit LobbyInterface(const dpp::slashcommand_t& event, const std::vector<Participant>& participants);
	~LobbyInterface() override;

    Participants participants;

    dpp::embed getEmbed() override;
    std::vector<dpp::component> getComponents() override;

    void onButtonClick(const dpp::button_click_t& buttonClick) override;
};