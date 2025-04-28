#include "model/command/command.h"

Command::Command(std::string name, std::string description, CommandOption option, std::vector<dpp::command_option> options)
{
    this->name = std::move(name);
    this->description = std::move(description);
    this->option = option;
    this->options = std::move(options);
}

void Command::onCommand(const dpp::cluster& client, const dpp::slashcommand_t& event)
{
    client.log(dpp::ll_info, "Command " + this->name + " hasn't onCommand function");
};