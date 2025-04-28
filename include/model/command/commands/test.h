#pragma once

#include <dpp/dpp.h>

#include "model/command/command.h"

struct Test : public Command
{
    Test() : Command("test", "Generate a test gif", CommandOption::NeedPlayer) {}

    void onCommand(const dpp::cluster& client, const dpp::slashcommand_t& event) override;

	static void createFight(const dpp::slashcommand_t& event);
};