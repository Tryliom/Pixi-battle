#pragma once

#include <dpp/dpp.h>

namespace Utils
{
	dpp::embed getGenericEmbed(uint32_t color, const std::string& title, const std::string& description = "");
	dpp::embed getNeutralEmbed(const std::string& title, const std::string& description = "");
	dpp::embed getGoodEmbed(const std::string& title, const std::string& description = "");
	dpp::embed getBadEmbed(const std::string& title, const std::string& description = "");
	dpp::embed getClosedEmbed();
	std::string formatStringToMaxCharacters(std::string str, int maxCharacters, bool addDots = true);
	unsigned int pos_of_char(unsigned char chr);
	std::string decode(std::string encoded_string);
	std::string toLowerCase(std::string str);
	std::string toUpperCase(std::string str);
	/**
	 * @brief Check if the std::string contains the pattern.
	 * @param str The std::string to check.
	 * @param pattern The pattern to check.
	 */
	bool contains(const std::string& str, const std::string& pattern);
	std::string replaceAll(std::string str, const std::string& from, const std::string& to);
}