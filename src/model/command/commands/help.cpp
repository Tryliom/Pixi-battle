#include "model/command/commands/help.h"

void Help::onCommand(const dpp::cluster& client, const dpp::slashcommand_t& event)
{
    dpp::embed embed = Utils::getNeutralEmbed("Help");

    for (const auto& command: CommandController::getCommands())
    {
        embed.add_field("/" + command->name, command->description, true);
    }

    event.reply(dpp::message().add_embed(embed));
}