#include "controller/mainController.h"

#include "controller/commandController.h"
#include "manager/userManager.h"
#include "model/user/user.h"

#include <iostream>
#include <dpp/nlohmann/json.hpp>

using json = nlohmann::json;

MainController::MainController() {
    json config_document;
    std::ifstream configfile("../config.json");
    configfile >> config_document;

    this->client = new dpp::cluster((config_document["token"]));
}

void MainController::start() const
{
    this->client->start_timer(dpp::timer_callback_t([&](dpp::timer)
	{
	    UserManager::getInstance().saveExistingUsers();
    }), 60 * 10);

    this->client->on_log(dpp::utility::cout_logger());
    this->client->on_slashcommand([&](const dpp::slashcommand_t& event)
	{
        try {
            CommandController::onCommand(*this->client, event);
        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    });
    this->client->on_ready([&](const dpp::ready_t& event)
	{
        if (dpp::run_once<struct bulk_register>()) {
            CommandController::refreshSlashCommands(*this->client);
        }
    });
    this->client->start(false);
}

void MainController::postGif(const std::string& gif, const std::function<void(const std::string& url)>& linkCallback) const
{
	dpp::message msg;
	msg.add_file("fight.gif", gif);
	msg.set_channel_id(836875771011399700);

	this->client->message_create(msg, [linkCallback](const dpp::confirmation_callback_t& callback) {
		if (callback.is_error())
		{
			std::cout << "Error, posting gif: " << callback.get_error().message << std::endl;
		}
		else
		{
			dpp::message message = std::get<dpp::message>(callback.value);
			std::string url = message.attachments[0].url;

			linkCallback(url);
		}
	});
}

void MainController::postImage(const std::string& image, const std::function<void(const std::string& url)>& linkCallback) const
{
	dpp::message msg;

	msg.add_file("image.png", image);
	msg.set_channel_id(836875771011399700);

	this->client->message_create(msg, [linkCallback](const dpp::confirmation_callback_t& callback) {
		if (callback.is_error())
		{
			std::cout << "Error, posting image: " << callback.get_error().message << std::endl;
		}
		else
		{
			dpp::message message = std::get<dpp::message>(callback.value);
			std::string url = message.attachments[0].url;

			linkCallback(url);
		}
	});
}

void MainController::postFight(const std::string& gifUrlData, const std::string& lastFrameUrlData, const std::function<void(const FightLink&)>& linkCallback) const
{
	this->postGif(gifUrlData, [linkCallback, this, lastFrameUrlData](const std::string& gifUrl) {
		this->postImage(lastFrameUrlData, [linkCallback, gifUrl](const std::string& lastFrameUrl) {
			linkCallback({ gifUrl, lastFrameUrl });
		});
	});
}
