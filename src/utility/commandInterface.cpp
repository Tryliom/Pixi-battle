#include "utility/commandInterface.h"

#include "controller/mainController.h"
#include "utility/utils.h"

#include <utility>

MenuBuilder::MenuBuilder(std::function<void(std::string value)> onMenuClick, std::function<std::vector<Option>()> getListOfOptions, std::string placeholder)
{
	this->onMenuClick = std::move(onMenuClick);
	this->getListOfOptions = std::move(getListOfOptions);
	this->placeholder = std::move(placeholder);
}

Content::Content(const dpp::embed& embed, std::vector<dpp::component> components)
{
	this->embed = embed;
	this->components = std::move(components);
}

CommandInterface::CommandInterface(dpp::slashcommand_t event, std::vector<MenuBuilder> menus, bool hasReply) :
	event(std::move(event))
{
	this->menus = std::move(menus);
	this->menuIndexes = {};
	this->errorMessage = "";
	this->confirmationMessage = "";
	this->page = 0;
	this->hasReply = hasReply;

	for (int i = 0; i < this->menus.size(); i++)
	{
		this->menuIndexes.emplace_back(0);
	}
}

void CommandInterface::start(bool assignCollector)
{
	this->updateMessage(nullptr);

	if (!assignCollector) return;

	this->buttonCollector = MainController::getInstance().client->on_button_click.attach([&](const dpp::button_click_t& buttonClick)
		{
			onButtonClickCallBack(buttonClick);
		}
	);

	this->menuCollector = MainController::getInstance().client->on_select_click.attach([&](const dpp::select_click_t& menuClick)
        {
			onMenuClickCallBack(menuClick);
        }
	);

	this->formCollector = MainController::getInstance().client->on_form_submit.attach([&](const dpp::form_submit_t& formClick)
       {
		   onFormClickCallBack(formClick);
       }
	);
}

bool CommandInterface::filter(const dpp::interaction& interaction)
{
	return std::to_string(interaction.message_id) == this->messageId && interaction.usr.id == this->event.command.usr.id;
}

void CommandInterface::updateMessage(Content* content)
{
	if (content == nullptr)
	{
		content = this->getContent();
	}

	dpp::message message = dpp::message();

	if (!this->errorMessage.empty())
	{
		message.add_embed(Utils::getBadEmbed("Error", this->errorMessage));
	}
	else if (!this->confirmationMessage.empty())
	{
		message.add_embed(Utils::getGoodEmbed("Confirmation", this->confirmationMessage));
	}
	else
	{
		for (const dpp::component& component: content->components)
		{
			message.add_component(component);
		}
		message.add_embed(content->embed);
	}

	if (!modalDisplayed)
	{
		if (!hasReply)
		{
			this->event.reply(message);

			hasReply = true;
		}
		else
		{
			if (this->lastInteraction != nullptr)
			{
				this->lastInteraction->reply([this, message](const dpp::confirmation_callback_t& confirmation)
                 {
                     this->event.edit_response(message);
                 }
				);
			}
			else
			{
				this->event.edit_response(message);
			}
		}

		if (this->messageId.empty())
		{
			this->event.get_original_response([this](const dpp::confirmation_callback_t& confirmation)
              {
                  this->messageId = std::to_string(get<dpp::message>(confirmation.value).id);
              });
		}
	}
	else
	{
		this->modalDisplayed = false;
	}
}

void CommandInterface::stopCollector(const dpp::embed& closeEmbed)
{
	// This make the whole bot loop into nothing
	/*MainController::getInstance().client->on_button_click.detach(this->buttonCollector);
	MainController::getInstance().client->on_select_click.detach(this->menuCollector);
	MainController::getInstance().client->on_form_submit.detach(this->formCollector);*/

	this->updateMessage(new Content(closeEmbed));
}

void CommandInterface::stopCollector() const
{
	// This make the whole bot loop into nothing
	/*MainController::getInstance().client->on_button_click.detach(this->buttonCollector);
	MainController::getInstance().client->on_select_click.detach(this->menuCollector);
	MainController::getInstance().client->on_form_submit.detach(this->formCollector);*/
}

Content* CommandInterface::getContent()
{
	return new Content(this->getEmbed(), this->getComponents());
}

dpp::embed CommandInterface::getEmbed()
{
	return Utils::getNeutralEmbed("Default embed");
}

std::vector<dpp::component> CommandInterface::getComponents()
{
	return {};
}

void CommandInterface::onButtonClick(const dpp::button_click_t& buttonClick)
{
	if (buttonClick.custom_id == "close")
	{
		this->stopCollector(Utils::getClosedEmbed());
	}
	else
	{
		this->updateMessage();
	}
}

void CommandInterface::onMenuClick(const dpp::select_click_t& menuClick)
{
	this->onMenuInteraction(menuClick);
	this->updateMessage();
}

void CommandInterface::onFormClick(const dpp::form_submit_t& menuClick)
{
	this->updateMessage();
}

void CommandInterface::onMenuPageChange(int index)
{}

void CommandInterface::setMenuPage(int index, int value)
{
	this->menuIndexes[index] = value;
}

void CommandInterface::updateMenuPageWithPage(int index, int page)
{
	int guessPage = 0;
	int minElements = 0;

	if (page < 24)
	{
		this->setMenuPage(index, 0);
	}
	else
	{
		minElements = 24;

		while (page >= minElements)
		{
			guessPage++;
			minElements += 22;
		}

		this->setMenuPage(index, guessPage);
	}
}

dpp::component CommandInterface::getCloseButton()
{
	return dpp::component().add_component(
		dpp::component().set_label("Close")
			.set_type(dpp::cot_button)
			.set_style(dpp::cos_secondary)
			.set_id("close")
	);
}

dpp::component CommandInterface::getChangePageButtons()
{
	return dpp::component().add_component(
		dpp::component().set_emoji("◀️")
			.set_type(dpp::cot_button)
			.set_style(dpp::cos_secondary)
			.set_id("changepage-back")
	).add_component(
		dpp::component().set_emoji("▶️")
			.set_type(dpp::cot_button)
			.set_style(dpp::cos_secondary)
			.set_id("changepage-next")
	);
}

std::vector<dpp::select_option> CommandInterface::getOptions(int page, std::vector<Option> list)
{
	std::vector<dpp::select_option> options;
	int minElements = 0;

	if (page > 0)
	{
		minElements = 24;
	}
	if (page > 1)
	{
		minElements += (page - 1) * 22;
	}

	if (page > 0)
	{
		options.emplace_back(dpp::select_option().set_label("Less..").set_emoji("◀️").set_value("less"));
	}

	for (int i = minElements; options.size() < 25; i++)
	{
		if (i < list.size())
		{
			dpp::select_option selectOption = dpp::select_option().set_label(Utils::formatStringToMaxCharacters(list[i].label, 100)).set_value(list[i].value);

			if (list[i].selected)
			{
				selectOption.set_default(true);
			}
			if (list[i].emoji != nullptr)
			{
				selectOption.set_emoji(list[i].emoji->name, list[i].emoji->id);
			}
			if (!list[i].description.empty())
			{
				selectOption.set_description(Utils::formatStringToMaxCharacters(list[i].description, 100));
			}

			options.emplace_back(selectOption);

			if (options.size() == 24)
			{
				options.emplace_back(dpp::select_option().set_label("More..").set_emoji("▶️").set_value("more"));
				break;
			}
		}
		else
		{
			break;
		}
	}

	return options;
}

void CommandInterface::onMenuInteraction(const dpp::select_click_t& menuClick)
{
	for (int i = 0; i < this->menus.size(); i++)
	{
		if (menuClick.custom_id == "menu-" + std::to_string(i))
		{
			if (menuClick.values[0] == "less")
			{
				this->updateMenuPageWithPage(i, this->menuIndexes[i] - 1);
			}
			else if (menuClick.values[0] == "more")
			{
				this->updateMenuPageWithPage(i, this->menuIndexes[i] + 1);
			}
			else
			{
				this->menus[i].onMenuClick(menuClick.values[0]);
			}
		}
	}
}

void CommandInterface::addMenuComponents(std::vector<dpp::component>& components, int index)
{
	for (int i = 0; i < this->menus.size(); i++)
	{
		if (index == -1 || i == index)
		{
			std::vector<dpp::select_option> options = this->getOptions(this->menuIndexes[i], this->menus[i].getListOfOptions());
			dpp::component component = dpp::component().set_type(dpp::cot_selectmenu)
				.set_id("menu-" + std::to_string(i))
				.set_placeholder(this->menus[i].placeholder)
				.set_min_values(1)
				.set_max_values(1);
			for (const dpp::select_option& option: options)
			{
				component.add_select_option(option);
			}

			components.emplace_back(dpp::component().add_component(component));
		}
	}
}

void CommandInterface::onButtonChangePage(const dpp::button_click_t& buttonClick, int maxPage, int index)
{
	if (buttonClick.custom_id == "changepage-back")
	{
		this->page--;
	}
	else if (buttonClick.custom_id == "changepage-next")
	{
		this->page++;
	}

	if (this->page > maxPage - 1)
	{
		this->page = 0;
	}
	else if (this->page < 0)
	{
		this->page = maxPage - 1;
	}

	if (buttonClick.custom_id.starts_with("changepage-"))
	{
		this->onChangePage();
		if (index != -1)
		{
			this->updateMenuPageWithPage(index, this->page);
		}
	}
}

void CommandInterface::onChangePage()
{}

void CommandInterface::displayErrorMessage(std::string message)
{
	this->errorMessage = std::move(message);
	this->updateMessage();
	MainController::getInstance().client->start_timer([this](dpp::timer timer)
		{
			this->errorMessage = "";
			this->updateMessage();
			MainController::getInstance().client->stop_timer(timer);
		},
		3
	);
}

void CommandInterface::displayConfirmationMessage(std::string message)
{
	this->confirmationMessage = std::move(message);
	this->updateMessage();
	MainController::getInstance().client->start_timer([this](dpp::timer timer)
		{
			this->confirmationMessage = "";
			this->updateMessage();
			MainController::getInstance().client->stop_timer(timer);
		},
		3
	);
}

void CommandInterface::displayModal(const dpp::interaction_modal_response& modal)
{
	this->lastInteraction->dialog(modal);
	this->modalDisplayed = true;
}

void CommandInterface::assignNewCallbacks(CommandInterface* otherCommandInterface)
{
	this->onButtonClickRedirect = [otherCommandInterface](const dpp::button_click_t& buttonClick){otherCommandInterface->onButtonClickCallBack(buttonClick);};
	this->onMenuClickRedirect = [otherCommandInterface](const dpp::select_click_t& menuClick){otherCommandInterface->onMenuClickCallBack(menuClick);};
	this->onFormClickRedirect = [otherCommandInterface](const dpp::form_submit_t& formClick){otherCommandInterface->onFormClickCallBack(formClick);};
}

void CommandInterface::onButtonClickCallBack(const dpp::button_click_t& buttonClick)
{
	if (this->filter(buttonClick.command))
	{
		if (onButtonClickRedirect != nullptr)
		{
			onButtonClickRedirect(buttonClick);

			return;
		}

		this->lastInteraction = new dpp::button_click_t(buttonClick);
		this->onButtonClick(buttonClick);
	}
}

void CommandInterface::onMenuClickCallBack(const dpp::select_click_t& menuClick)
{
	if (this->filter(menuClick.command))
	{
		if (onMenuClickRedirect != nullptr)
		{
			onMenuClickRedirect(menuClick);

			return;
		}

		this->lastInteraction = new dpp::select_click_t(menuClick);
		this->onMenuClick(menuClick);
	}
}

void CommandInterface::onFormClickCallBack(const dpp::form_submit_t& formClick)
{
	if (this->filter(formClick.command))
	{
		if (onFormClickRedirect != nullptr)
		{
			onFormClickRedirect(formClick);

			return;
		}

		this->lastInteraction = new dpp::form_submit_t(formClick);
		this->onFormClick(formClick);
	}
}

CommandInterface::~CommandInterface()
{
	delete this->lastInteraction;
}
