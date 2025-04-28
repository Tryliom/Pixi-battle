#include "utility/description.h"

#include "manager/iconManager.h"
#include "model/entity/skill/skill.h"
#include "utility/utils.h"

std::string Description::asString(Skill* skill)
{
	std::string desc = this->description;

	if (skill != nullptr)
	{
		int i = 0;
		for (SkillParameter parameter: this->parameters)
		{
			std::string str = "skill" + std::to_string(i);
			desc.replace(
				desc.find(str),
				std::string(str).size(),
				std::to_string(
					parameter.defaultValue + skill->getImprovementTotalValueForDisplay(parameter.type)
				)
			);
			i++;
		}
	}

	int i = 0;
	for (EffectList effect: this->effects)
	{
		std::string str = "effect" + std::to_string(i);
		EffectData data = EffectFactory::getEffectData(effect);
		dpp::emoji* emoji = IconManager::getEmoji(data.emojiName);
		std::string name = Utils::toLowerCase(data.name);

		if (emoji != nullptr)
		{
			name += " " + emoji->get_mention();
		}

		desc.replace(desc.find(str), std::string(str).size(), "**" + name + "**");
		i++;
	}

	return desc;
}