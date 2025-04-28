#pragma once

#include "model/command/command.h"

#include "utility/commandInterface.h"

struct ClassType;
struct User;
struct Entity;

enum class CreateScreen
{
	main = 0,
	skills = 1,
	classes = 2
};

struct Create : public Command
{
	Create() : Command("create", "Create a new character", CommandOption::NeedDeadPlayer) {}

	void onCommand(const dpp::cluster& client, const dpp::slashcommand_t& event) override;
};

struct CreateInterface : public CommandInterface
{
    explicit CreateInterface(const dpp::slashcommand_t& event);

	CreateScreen screen = CreateScreen::main;
	User& user;

	std::vector<ClassType*> unlockedClasses;

	int rerollLeft = 3;
	int currentClass = 0;
	bool defaultClass = true;
	bool simplifiedView = false;

	void changeScreen(CreateScreen newScreen);

	dpp::embed getEmbed() override;
	std::vector<dpp::component> getComponents() override;

	void onButtonClick(const dpp::button_click_t& buttonClick) override;
	void onMenuClick(const dpp::select_click_t& menuClick) override;
	void onFormClick(const dpp::form_submit_t& formClick) override;

	[[nodiscard]] int getMaxPage() const;
};
