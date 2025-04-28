#include "model/command/commands/test.h"

#include "model/entity/entity.h"
#include "model/entity/player.h"
#include "model/entity/fightState.h"
#include "model/entity/classType/classType.h"

#include "model/image/gif.h"
#include "model/user/user.h"
#include "model/fight/participant.h"

#include "manager/skillManager.h"
#include "manager/userManager.h"

#include "utility/timeDifference.h"
#include "controller/mainController.h"
#include "model/entity/monster.h"
#include "interfaces/lobbyInterface.h"

void Test::onCommand(const dpp::cluster& client, const dpp::slashcommand_t& event)
{
	std::cout << "Test command" << std::endl;

	event.reply(dpp::message().add_embed(Utils::getNeutralEmbed("Loading")), [event](const dpp::confirmation_callback_t &callback) {
		if (callback.is_error())
		{
			std::cout << callback.get_error().message << ", code: " << callback.get_error().code << std::endl;
		}
		else
		{
            createFight(event);
		}
	});
}

void Test::createFight(const dpp::slashcommand_t& event)
{
	User& user = UserManager::getInstance().getUser(std::to_string(event.command.usr.id));

	user.player->updateLevel(30);

	std::vector<Participant> participants = {
		Participant(user.player, Enums::Team::Left, Enums::Slot::TopLeft),
		//Participant(new Monster("Monster 1", new Slime()), Enums::Team::Left, Enums::Slot::TopRight),
		Participant(new Monster("Monster 2", new Thief()), Enums::Team::Left, Enums::Slot::BottomLeft),
		//Participant(new Monster("Monster 3", new Slime()), Enums::Team::Left, Enums::Slot::BottomRight),
		//Participant(new Monster("Monster 4", new Thief()), Enums::Team::Right, Enums::Slot::TopLeft),
		//Participant(new Monster("Monster 5", new Slime()), Enums::Team::Right, Enums::Slot::TopRight),
		//Participant(new Monster("Monster 6", new Thief()), Enums::Team::Right, Enums::Slot::BottomLeft),
		Participant(new Monster("Monster 7", new Slime()), Enums::Team::Right, Enums::Slot::BottomRight)
	};

    Participants participantCollection(participants);
    (new LobbyInterface(event, participants))->start();

    return;

	Gif gif;
	TimeDifference td;
	participantCollection.initialize();

	while (!participantCollection.isFightFinished())
	{
		participantCollection.onFrame(gif);

		gif.addAndInitializeNewImage();
	}

	for (auto& participant : participantCollection.participants)
	{
		participant.entity->onEndFight();
	}

	gif.addImage();
	std::cout << "Time to generate a gif: " << td.getDifferenceWithStart() << "ms" << std::endl;
	MainController::getInstance().postGif(gif.generate(), [td, event](const std::string& link) {
		event.edit_response(dpp::message().add_embed(Utils::getNeutralEmbed("Fight test").set_image(link)));
		UserManager::getInstance().saveExistingUsers();

		std::cout << "Whole process: " << td.getDifferenceWithStart() << "ms" << std::endl;
	});
}
