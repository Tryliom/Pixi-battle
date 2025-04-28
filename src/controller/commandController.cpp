#include "controller/commandController.h"

#include "model/command/commands/help.h"
#include "model/command/commands/test.h"
#include "model/command/commands/create.h"

#include "manager/userManager.h"

#include "model/user/user.h"
#include "model/entity/player.h"

namespace CommandController
{
    std::vector<Command*> getCommands()
    {
        return {
            new Help(),
            new Test(),
            new Create()
        };
    }

    void onCommand(const dpp::cluster& client, const dpp::slashcommand_t& event)
    {
        for (const auto& command: getCommands())
        {
            if (event.command.get_command_name() == command->name)
            {
				UserManager& userManager = UserManager::getInstance();
				std::string userID = std::to_string(event.command.usr.id);

                if (command->option != CommandOption::None)
                {
                    userManager.createUser(userID);
                }
                if (command->option == CommandOption::NeedDeadPlayer && event.command.usr.id != static_cast<dpp::snowflake>(474294290508546069))
                {
                    User& user = userManager.getUser(userID);
                    if (user.player != nullptr && !user.player->dead)
                    {
                        event.reply(dpp::message().add_embed(Utils::getBadEmbed("Error", "You need to have your character dead to use this command")));
                        return;
                    }
                }
                if (command->option == CommandOption::NeedPlayer)
                {
                    User& user = userManager.getUser(userID);

                    if (user.player == nullptr || user.player->dead)
                    {
                        event.reply(dpp::message().add_embed(Utils::getBadEmbed("Error", "You need to have your an alive character to use this command")));
                        return;
                    }
                }

                command->onCommand(client, event);
            }
        }
    };

    void refreshSlashCommands(dpp::cluster& bot)
    {
        std::vector<dpp::slashcommand> slash_commands;

        for (const auto& command: getCommands())
        {
            dpp::slashcommand slashCommand;
            slashCommand.set_name(command->name)
                    .set_description(Utils::formatStringToMaxCharacters(command->description, 100))
                    .set_application_id(bot.me.id);
            slashCommand.options = command->options;
            slash_commands.emplace_back(slashCommand);
        }

        bot.global_bulk_command_create(slash_commands);
    }
}