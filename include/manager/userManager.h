#pragma once

#include <map>
#include <string>
#include <dpp/nlohmann/json.hpp>

struct User;

struct UserManager final
{
private:
	UserManager();

	std::map<std::string, User> _users;
	std::vector<std::string> _existingUsers;

public:
	UserManager(const UserManager&) = delete;
	UserManager& operator=(const UserManager&) = delete;
	UserManager(UserManager&&) = delete;
	UserManager& operator=(UserManager&&) = delete;

	static auto& getInstance()
	{
		static UserManager instance;
		return instance;
	}

	void createUser(const std::string& id);
	User& getUser(const std::string& id);
	[[nodiscard]] bool userExists(const std::string& id) const;

	void saveExistingUsers();
	void loadExistingUsers();
	void loadUser(const std::string& id);

	void fromJson(const nlohmann::json& user);
};
