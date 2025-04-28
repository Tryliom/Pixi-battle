#include "interfaces/lobbyInterface.h"

#include "model/fight/participants.h"

#include "model/entity/entity.h"
#include "model/entity/classType/classType.h"
#include "manager/iconManager.h"
#include "interfaces/fightInterface.h"
#include "controller/mainController.h"

LobbyInterface::LobbyInterface(const dpp::slashcommand_t& event, const std::vector<Participant>& participants) :
    CommandInterface(event, {}, true), participants(participants)
{

}

dpp::embed LobbyInterface::getEmbed()
{
    auto participantDisplayOrder = participants.getDisplayOrder();
    dpp::embed embed = Utils::getNeutralEmbed("Lobby");
    std::vector<std::string> leftTeam;
    std::vector<std::string> rightTeam;
    int leftTeamSize = 0;
    int rightTeamSize = 0;

    for (auto& participant : participantDisplayOrder)
    {
        if (participant.newLine) continue;

        std::string displayName = IconManager::getEmoji("available_spot")->get_mention() + " Empty";

        if (participant.index != -1) {
            Participant& participantRef = participants.getParticipant(participant.index);
			dpp::emoji* emoji = IconManager::getEmoji(participantRef.entity->classType->emojiName);

			if (emoji == nullptr)
			{
				// Replace emojiName by the default one
				emoji = IconManager::getEmoji("default");
			}

            displayName = emoji->get_mention() + " " + participantRef.entity->name + " **lvl " + std::to_string(participantRef.entity->classType->level) + "**";

            if (participant.displayOrder.team == Enums::Team::Left)
            {
                leftTeamSize++;
            }
            else
            {
                rightTeamSize++;
            }
        }

        if (participant.displayOrder.team == Enums::Team::Left)
        {
            leftTeam.emplace_back(displayName);
        }
        else
        {
            rightTeam.emplace_back(displayName);
        }
    }

    auto transform = [](std::vector<std::string>& team)
    {
        std::string teamString;

        int i = 0;
        for (const auto& str : team)
        {
            teamString += str;

            if (i % 2 == 0)
            {
                teamString += " | ";
            }

            if (i == 1)
            {
                teamString += "\n";
            }

            i++;
        }

        return teamString;
    };

    embed.add_field("Team 1 [" + std::to_string(leftTeamSize) + "/4]", transform(leftTeam), true);
    embed.add_field("Team 2 [" + std::to_string(rightTeamSize) + "/4]", transform(rightTeam), true);

    return embed;
}

std::vector<dpp::component> LobbyInterface::getComponents()
{
	std::vector<dpp::component> components;

	participants.addParticipantsButtons(components, [](dpp::component& button, ParticipantOrder order)
	{
		button.set_disabled(order.displayOrder.team == Enums::Team::Right || order.index != -1);

		if (order.index == -1)
		{
			auto* emoji = IconManager::getEmoji("available_spot");
			button.set_emoji(emoji->name, emoji->id);
		}
	});

	auto row = dpp::component();
	row.set_type(dpp::cot_action_row);

	row.add_component(dpp::component().set_id("start").set_type(dpp::cot_button).set_style(dpp::cos_success).set_label("Start"));
	row.add_component(dpp::component().set_id("cancel").set_type(dpp::cot_button).set_style(dpp::cos_danger).set_label("Cancel"));

	components.emplace_back(row);

    return components;
}

void LobbyInterface::onButtonClick(const dpp::button_click_t& buttonClick)
{
	// Only the user can move in the interface, if he clicks on a button, it's to move a character

	if (buttonClick.custom_id == "start")
	{
		auto* fightInterface = new FightInterface(event, participants);

		this->assignNewCallbacks(fightInterface);

		fightInterface->start(false);

		return;
	}
	else if (buttonClick.custom_id == "cancel")
	{
		// Stop the lobby
		this->stopCollector(Utils::getClosedEmbed());
	}
	else if (buttonClick.custom_id.starts_with("slot-"))
	{
		auto participantDisplayOrder = participants.getDisplayOrder();
		int slotIndex = std::stoi(buttonClick.custom_id.substr(5));

		// You can only move slot to slot for the moment
		if (participantDisplayOrder[slotIndex].index == -1)
		{
			// Find the first slot with a character
			for (int i = 0; i < participantDisplayOrder.size(); i++)
			{
				if (participantDisplayOrder[i].index != -1)
				{
					Participant& participant = participants.getParticipant(participantDisplayOrder[i].index);

					if (participant.entity->type == Enums::Type::Entity::Player)
					{
						// Move the character to the slot
						participants.moveParticipant(participantDisplayOrder[i].displayOrder, participantDisplayOrder[slotIndex].displayOrder);
						break;
					}
				}
			}
		}
	}

    CommandInterface::onButtonClick(buttonClick);
}

LobbyInterface::~LobbyInterface()
{
	CommandInterface::~CommandInterface();

	std::cout << "LobbyInterface destructor" << std::endl;


}
