#include "manager/iconManager.h"

namespace IconManager
{
    dpp::emoji* getEmoji(const std::string& emojiName)
    {
        std::vector<dpp::snowflake> guilds = {833610358286450748, 1011612500401344552, 1011614625214771200};

		for (dpp::snowflake guildID: guilds)
        {
            dpp::guild* guild = dpp::find_guild(guildID);

            for (dpp::snowflake id: guild->emojis)
            {
                dpp::emoji* emoji = dpp::find_emoji(id);

                if (emoji->name == emojiName)
                {
                    return emoji;
                }
            }
        }

        return nullptr;
    }
}