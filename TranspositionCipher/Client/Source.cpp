#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <string>
#include <numeric>
#include <sstream>
#include <regex>
#include <unordered_set>
#include <cpr/cpr.h>
#include <crow.h>

std::string Encryption(std::string message, std::string keyInput)
{
	std::string encryptionMessage;
	std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
	std::vector<std::vector<std::string>> matrix;
	std::vector < std::tuple<std::string, int, int >> key; // symbol, position alphabet, position column

	for (int index = 0; index < keyInput.size(); index++)
	{
		char current_char = keyInput[index];
		int alphabet_pos = alphabet.find(tolower(current_char));
		key.push_back({ std::string(1, current_char), alphabet_pos, index });
	}

	int size = message.size() / key.size();
	if (message.size() % key.size() != 0)
		size++;
	for (int index = 0; index < size; index++)
	{
		std::vector<std::string> line;
		for (int jndex = 0; jndex < key.size(); jndex++)
		{
			if (!message.empty())
			{
				line.push_back(std::string(1, message[0]));
				message.erase(message.begin());
			}
			else
			{
				line.push_back(std::string(1, alphabet[0]));
				alphabet.erase(alphabet.begin());
			}
		}
		matrix.push_back(line);
	}

	std::sort(key.begin(), key.end(), [](const std::tuple<std::string, int, int>& a, const std::tuple<std::string, int, int>& b)
		{
			return std::get<1>(a) < std::get<1>(b);
		});

	for (int jndex = 0; jndex < key.size(); jndex++)
	{
		for (int index = 0; index < size; index++)
		{
			encryptionMessage += matrix[index][std::get<2>(key[jndex])];
		}
	}
	return encryptionMessage;
}

bool VerifyKey(std::string key)
{
	std::regex pattern("^[a-zA-Z]+$");
	std::unordered_set<char> seenCharacters;
	std::string result;
	for (char character : key) 
	{
		result += std::tolower(static_cast<unsigned char>(character)); 
	}
	for (char character : result) 
	{
		if (seenCharacters.find(character) != seenCharacters.end()) 
		{
			return false;
		}
		seenCharacters.insert(character);
	}
	return regex_match(key,pattern);
}

int main() {
	std::string message, key;
	while (1)
	{
		std::cout << "Introduce the key:\n";
		std::getline(std::cin, key);
		if (VerifyKey(key))
		{
			std::cout << "Introduce the message:\n";
			std::getline(std::cin, message);
			std::string encryptionMessage = Encryption(message, key);
			auto response = cpr::Put(
				cpr::Url{ "http://localhost:18080/message" },
				cpr::Parameters{
						{ "message",encryptionMessage},
						{ "key", key},
				}
			);
			if (response.status_code == 200 || response.status_code == 201) {
				std::cout << "The message was sent successfully.\n";
			}
			else {
				std::cout << "There was a problem.\n";
			}
		}
		else
		{
			std::cout << "Introduce the key again.\n";
		}
	}
	return 0;
}
