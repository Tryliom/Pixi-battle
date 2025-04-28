#pragma once

#include "dpp/dpp.h"

#include "utility/utils.h"
#include "model/command/command.h"
#include "controller/commandController.h"

struct Help : public Command
{
    Help() : Command("help", "Display the help about others commands", CommandOption::None) {}

    void onCommand(const dpp::cluster& client, const dpp::slashcommand_t& event) override;
};