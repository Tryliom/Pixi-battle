#pragma once

#include <string>
#include <utility>

#include <dpp/dpp.h>

enum class CommandOption
{
    NeedUser = 0,
    NeedPlayer = 1,
    NeedDeadPlayer = 2,
    None = 3
};

struct Command
{
    Command(std::string name, std::string description, CommandOption option = CommandOption::None, std::vector<dpp::command_option> options = {});

    std::string name;
    std::string description;
    CommandOption option;
    std::vector<dpp::command_option> options;

    virtual void onCommand(const dpp::cluster& client, const dpp::slashcommand_t& event);
};