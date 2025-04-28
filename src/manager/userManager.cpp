#include "manager/userManager.h"

#include <fstream>
#include <dpp/nlohmann/json.hpp>
#include <iostream>

#include "model/user/user.h"

#include "model/entity/player.h"

using json = nlohmann::json;

UserManager::UserManager()
{
    this->_users = {};
	this->_existingUsers = {};
}

void UserManager::createUser(const std::string& id)
{
	if (!this->userExists(id))
	{
		this->_users.emplace(id, User(id));
		this->_existingUsers.emplace_back(id);

		std::cout << "Created user " << id << std::endl;
	}
    else if (!this->_users.contains(id))
    {
        this->loadUser(id);

		std::cout << "Loaded user " << id << std::endl;
    }
}

User& UserManager::getUser(const std::string& id)
{
	User& user = this->_users[id];

	user.updateLastInteraction();

    return user;
}

void UserManager::saveExistingUsers()
{
	std::vector<std::string> userToFreed;

	for (const auto& user: this->_users)
	{
		try
		{
			std::ofstream o("../assets/data/users/" + user.first + ".json");
			json j = user.second;
			o << j << std::endl;
		}
		catch (std::exception& e)
		{
			std::cout << "Cannot save user " << user.first << ": " << e.what() << std::endl;
		}

		if (user.second.isFreeable())
		{
			userToFreed.emplace_back(user.first);
		}
	}

	for (const std::string& userId: userToFreed)
	{
		this->_users.erase(userId);

		std::cout << "User " << userId << " has been freed" << std::endl;
	}
}

void UserManager::loadExistingUsers()
{
	std::string path = "../assets/data/users/";

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		std::string fileName = entry.path().filename().string();
		std::string userId = fileName.substr(0, fileName.find_last_of('.'));

		std::cout << "Found user " << userId << std::endl;

		this->_existingUsers.emplace_back(userId);
	}
}

void UserManager::fromJson(const json& user)
{
	User newUser = user;
	newUser.player->setUser(newUser.id);
	this->_users.emplace(newUser.id, newUser);
}

bool UserManager::userExists(const std::string& id) const
{
	return std::find(this->_existingUsers.begin(), this->_existingUsers.end(), id) != this->_existingUsers.end();
}

void UserManager::loadUser(const std::string& id)
{
	try
	{
		std::ifstream i("../assets/data/users/" + id + ".json");
		json j = json::parse(i);
		this->fromJson(j);

		std::cout << "User " << id << " has been loaded" << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << "Cannot load user " << id << ": " << e.what() << std::endl;
	}
}
