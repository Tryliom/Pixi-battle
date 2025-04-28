#pragma once

#include <dpp/cluster.h>
#include "model/image/gif.h"

struct FightLink
{
	std::string gifUrl;
	std::string lastFrameUrl;
};

struct MainController
{
private:
	MainController();

public:
	MainController(const MainController&) = delete;
	MainController& operator=(const MainController&) = delete;
	MainController(MainController&&) = delete;
	MainController& operator=(MainController&&) = delete;

    dpp::cluster* client;

	static auto& getInstance()
	{
		static MainController instance;
		return instance;
	}

	void start() const;

	void postGif(const std::string& gif, const std::function<void(const std::string& url)>& linkCallback) const;
	void postImage(const std::string& image, const std::function<void(const std::string& url)>& linkCallback) const;

	void postFight(const std::string& gifUrlData, const std::string& lastFrameUrlData, const std::function<void(const FightLink& link)>& linkCallback) const;

	//TODO: Add saveExistingUsers and loadExistingUsers functions for the user manager
};
