#include <filesystem>
#include <iostream>
#include <memory>

#include <crow.h>

std::string Decryption(std::string message, std::string keyInput)
{
	std::string decryptionMessage;
	std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
	std::vector<std::vector<std::string>> matrix, matrixCopy;
	std::vector < std::tuple<std::string, int, int >> key; // symbol, position alphabet, position column

	for (int index = 0; index < keyInput.size(); index++)
	{
		char current_char = keyInput[index];
		int alphabet_pos = alphabet.find(tolower(current_char));
		key.push_back({ std::string(1, current_char), alphabet_pos, index });
	}

	auto keyCopy = key;
	std::sort(keyCopy.begin(), keyCopy.end(), [](const std::tuple<std::string, int, int>& a, const std::tuple<std::string, int, int>& b)
		{
			return std::get<1>(a) < std::get<1>(b);
		});
	for (int index = 0; index < key.size(); index++)
	{
		std::get<1>(keyCopy[index]) = index+1;
	
	}
	for (int index = 0; index < key.size(); index++)
	{
		for (int jndex = 0; jndex < keyCopy.size(); jndex++)
		{
			if (std::get<0>(key[index]) == std::get<0>(keyCopy[jndex]))
			{
				std::get<1>(key[index]) = std::get<1>(keyCopy[jndex]);
				break;
			}
		}
	}

	int size = message.size() / key.size();
	for (int index = 0; index < key.size(); index++)
	{
		std::vector<std::string> line;
		int start = (std::get<1>(key[index])-1) * size;
		std::string substring = message.substr(start,size);
		for (int jndex = 0; jndex < substring.size(); jndex++)
		{
			line.push_back(std::string(1, substring[jndex]));
		}
		matrix.push_back(line);
	}

	std::sort(key.begin(), key.end(), [](const std::tuple<std::string, int, int>& a, const std::tuple<std::string, int, int>& b)
		{
			return std::get<1>(a) < std::get<1>(b);
		});

	for (int index = 0; index < size; index++)
	{
		for (int jndex = 0; jndex < key.size(); jndex++)
		{
			decryptionMessage = decryptionMessage + matrix[jndex][index];
		}
	}

	return decryptionMessage;
}

int main()
{
	crow::SimpleApp app;
	std::string message, key;
	CROW_ROUTE(app, "/")([]() {
		return "Server is running. This is the main branch";
		});
	CROW_ROUTE(app, "/message")
		.methods(crow::HTTPMethod::PUT)
		([&message, &key](const crow::request& req) {
		std::string receivedMessage{ req.url_params.get("message") };
		std::string receivedKey{ req.url_params.get("key") };
		message = receivedMessage;
		key = receivedKey;
		std::cout << "The message before decryption is:" << message << "\n";
		message = Decryption(message, key);
		std::cout << "The message after decryption is:" << message << "\n";
		return 200;
			});
	app.port(18080).multithreaded().run();
	return 0;
}