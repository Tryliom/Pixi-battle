#pragma once

#include "dpp/dpp.h"

#include "model/command/command.h"

namespace CommandController
{
	std::vector<Command*> getCommands();
	void onCommand(const dpp::cluster& client, const dpp::slashcommand_t& event);
	void refreshSlashCommands(dpp::cluster& bot);
}