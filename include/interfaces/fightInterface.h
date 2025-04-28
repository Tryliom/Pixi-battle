#pragma once

#include "utility/commandInterface.h"

struct Participants;

enum class Menu
{
	LOADING, // Loading gif and waiting for the fight to start
	START_TURN, // Displayed when a player start his turn with multiple actions
	CHOOSE_SKILLS, // Displayed when a player need to choose a skill
	CHOOSE_TARGET, // Displayed when a player need to choose a target
	CHOOSE_ENTITY_PART, // Displayed when a player need to choose a part of an entity to attack, only used with a damage skill single target
	INSPECT_MENU, // Displayed when a player want to see active and passive skills of other players/entities
	INSPECT_ENTITY, // Displayed when a player inspect an entity
	END_FIGHT, // Displayed when the fight is finished, before going to rewards
	FINISH_TURN, // Used to run continueGif() after the player has chosen his actions
};

struct FightInterface : public CommandInterface
{
	explicit FightInterface(const dpp::slashcommand_t& event, Participants& participants);

	Participants& participants;

	std::string gifUrl;
	std::string lastFrameUrl;

	Menu menu = Menu::LOADING;

	// Current skill being used
	int currentSkillIndex = 0;

	// Current target being used
	int currentTargetIndex = 0;

	// Called after update, while loading is true
	void continueGif();

	void start(bool assignCollector = true) override;

	dpp::embed getEmbed() override;
	std::vector<dpp::component> getComponents() override;
	void onButtonClick(const dpp::button_click_t& buttonClick) override;

	// Start turn
	dpp::embed getStartTurnEmbed(dpp::embed& embed);
	static std::vector<dpp::component> getStartTurnComponents();
	void onStartTurnButtonClick(const dpp::button_click_t& buttonClick);

	// Choose skills
	dpp::embed getChooseSkillsEmbed(dpp::embed& embed);
	std::vector<dpp::component> getChooseSkillsComponents();
	void onChooseSkillsButtonClick(const dpp::button_click_t& buttonClick);
	void onChooseSkillsSelect(const std::string& value);
	std::vector<Option> getChooseSkillsOptions();

	// Choose target
	dpp::embed getChooseTargetEmbed(dpp::embed& embed);
	std::vector<dpp::component> getChooseTargetComponents();
	void onChooseTargetButtonClick(const dpp::button_click_t& buttonClick);

	// Choose entity part
	dpp::embed getChooseEntityPartEmbed(dpp::embed& embed);
	std::vector<dpp::component> getChooseEntityPartComponents();
	void onChooseEntityPartButtonClick(const dpp::button_click_t& buttonClick);

	// Inspect menu
	dpp::embed getInspectMenuEmbed(dpp::embed& embed);
	std::vector<dpp::component> getInspectMenuComponents();
	void onInspectMenuButtonClick(const dpp::button_click_t& buttonClick);

	// Inspect entity
	dpp::embed getInspectEntityEmbed(dpp::embed& embed);
	std::vector<dpp::component> getInspectEntityComponents();
	void onInspectEntityButtonClick(const dpp::button_click_t& buttonClick);

	// End fight
	dpp::embed getEndFightEmbed(dpp::embed& embed);
	std::vector<dpp::component> getEndFightComponents();
	void onEndFightButtonClick(const dpp::button_click_t& buttonClick);

	// Utility
	static std::string getEmojiMention(const std::string& emojiName);
    static dpp::emoji* getEmoji(const std::string& emojiName);
    static dpp::component getSecondaryButton();
    static dpp::component getPrimaryButton();
};