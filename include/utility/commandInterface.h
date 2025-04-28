#pragma once

#include <vector>
#include <dpp/dpp.h>

struct Option
{
	std::string label;
	std::string description;
	std::string value;
	dpp::emoji* emoji{ nullptr };
	bool selected{ false };
};

struct MenuBuilder
{
	MenuBuilder(std::function<void(std::string value)> onMenuClick, std::function<std::vector<Option>()> getListOfOptions, std::string placeholder);

	std::function<void(std::string value)> onMenuClick;
	std::function<std::vector<Option>()> getListOfOptions;
	std::string placeholder;
};

struct Content
{
	explicit Content(const dpp::embed& embed, std::vector<dpp::component> components = {});

	dpp::embed embed;
	std::vector<dpp::component> components;
};

struct CommandInterface
{
	explicit CommandInterface(dpp::slashcommand_t event, std::vector<MenuBuilder> menus = {}, bool hasReply = false);
	virtual ~CommandInterface();

	dpp::slashcommand_t event;
	dpp::interaction_create_t* lastInteraction{nullptr};
	std::string messageId;
	std::vector<MenuBuilder> menus;
	std::vector<int> menuIndexes;

	std::string errorMessage;
	std::string confirmationMessage;

	int page = 0;
	bool modalDisplayed{ false };
	bool hasReply { false };

	dpp::event_handle buttonCollector{};
	dpp::event_handle menuCollector{};
	dpp::event_handle formCollector{};

	std::function<void(const dpp::button_click_t& buttonClick)> onButtonClickRedirect = nullptr;
	std::function<void(const dpp::select_click_t& menuClick)> onMenuClickRedirect = nullptr;
	std::function<void(const dpp::form_submit_t& formClick)> onFormClickRedirect = nullptr;

	void assignNewCallbacks(CommandInterface* otherCommandInterface);

	virtual void start(bool assignCollector = true);
	bool filter(const dpp::interaction& interaction);
	void updateMessage(Content* content = nullptr);

	Content* getContent();
	virtual dpp::embed getEmbed();
	virtual std::vector<dpp::component> getComponents();

	virtual void onButtonClick(const dpp::button_click_t& buttonClick);
	virtual void onMenuClick(const dpp::select_click_t& menuClick);
	virtual void onFormClick(const dpp::form_submit_t& formClick);

	void stopCollector(const dpp::embed& closeEmbed);
	void stopCollector() const;
	/**
	 * Use this function to do something when a user clicks on a menu option. Not trigger by less and more options.
	 * @param index The index of the menu option.
	 */

	void onMenuPageChange(int index);
	void setMenuPage(int index, int value);
	void updateMenuPageWithPage(int index, int page);

	static dpp::component getCloseButton();
	static dpp::component getChangePageButtons();
	static std::vector<dpp::select_option> getOptions(int page, std::vector<Option> list);

	void onMenuInteraction(const dpp::select_click_t& menuClick);
	void addMenuComponents(std::vector<dpp::component>& components, int index = -1);

    // Call it in onButtonClick if implemented
	void onButtonChangePage(const dpp::button_click_t& buttonClick, int maxPage, int index = -1);
	void onChangePage();

	void displayErrorMessage(std::string message);
	void displayConfirmationMessage(std::string message);
	void displayModal(const dpp::interaction_modal_response& modal);

	void onButtonClickCallBack(const dpp::button_click_t& buttonClick);
	void onMenuClickCallBack(const dpp::select_click_t& menuClick);
	void onFormClickCallBack(const dpp::form_submit_t& formClick);
};
