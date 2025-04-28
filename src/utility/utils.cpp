#include <dpp/dpp.h>
#include <regex>
#include <random>

namespace Utils
{
	dpp::embed getGenericEmbed(uint32_t color, const std::string& title, const std::string& description = "")
	{
		return dpp::embed().set_color(color).set_title(title).set_description(description);
	}

	dpp::embed getNeutralEmbed(const std::string& title, const std::string& description = "")
	{
		return getGenericEmbed(0x0099ff, title, description);
	}

	dpp::embed getGoodEmbed(const std::string& title, const std::string& description = "")
	{
		return getGenericEmbed(0x11aa11, title, description);
	}

	dpp::embed getBadEmbed(const std::string& title, const std::string& description = "")
	{
		return getGenericEmbed(0xaa1111, title, description);
	}

	dpp::embed getClosedEmbed()
	{
		return getGenericEmbed(0x6b858e, "Closed", "This menu has been closed");
	}

	std::string formatStringToMaxCharacters(std::string str, int maxCharacters, bool addDots)
	{
		if (str.size() > maxCharacters)
		{
			if (addDots)
			{
				str.resize(maxCharacters - 2);
				str.append("..");
			}
			else
			{
				str.resize(maxCharacters);
			}
		}
		return str;
	}

	unsigned int pos_of_char(const unsigned char chr)
	{
		if (chr >= 'A' && chr <= 'Z')
		{ return chr - 'A'; }
		else if (chr >= 'a' && chr <= 'z')
		{ return chr - 'a' + ('Z' - 'A') + 1; }
		else if (chr >= '0' && chr <= '9')
		{ return chr - '0' + ('Z' - 'A') + ('z' - 'a') + 2; }
		else if (chr == '+' || chr == '-')
		{ return 62; }
		else if (chr == '/' || chr == '_')
		{ return 63; }
		else
		{
			throw std::runtime_error("Input is not valid base64-encoded data.");
		}
	}

	std::string decode(std::string encoded_string)
	{
		size_t length_of_string = encoded_string.length();
		size_t pos = 0;
		size_t approx_length_of_decoded_string = length_of_string / 4 * 3;
		std::string ret;
		ret.reserve(approx_length_of_decoded_string);

		while (pos < length_of_string)
		{
			size_t pos_of_char_1 = pos_of_char(encoded_string[pos + 1]);

			ret.push_back(static_cast<std::string::value_type>(((pos_of_char(encoded_string[pos + 0])) << 2) + ((pos_of_char_1 & 0x30) >> 4)));

			if ((pos + 2 < length_of_string) && encoded_string[pos + 2] != '=' && encoded_string[pos + 2] != '.')
			{
				unsigned int pos_of_char_2 = pos_of_char(encoded_string[pos + 2]);
				ret.push_back(static_cast<std::string::value_type>(((pos_of_char_1 & 0x0f) << 4) + ((pos_of_char_2 & 0x3c) >> 2)));

				if ((pos + 3 < length_of_string) && encoded_string[pos + 3] != '=' && encoded_string[pos + 3] != '.')
				{
					ret.push_back(static_cast<std::string::value_type>(((pos_of_char_2 & 0x03) << 6) + pos_of_char(encoded_string[pos + 3])));
				}
			}

			pos += 4;
		}

		return ret;
	}

	std::string toLowerCase(std::string str)
	{
		std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
		{ return std::tolower(c); });
		return str;
	}

	std::string toUpperCase(std::string str)
	{
		std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
		{ return std::toupper(c); });
		return str;
	}


	bool contains(const std::string& str, const std::string& pattern)
	{
		std::regex r(pattern);
		return std::regex_search(str, r);
	}

	std::string replaceAll(std::string str, const std::string& from, const std::string& to)
	{
		if (from.empty())
		{
			return str;
		}

		size_t start_pos = 0;

		while ((start_pos = str.find(from, start_pos)) != std::string::npos)
		{
			str.replace(start_pos, from.length(), to);
			start_pos += to.length();
		}

		return str;
	}
}
