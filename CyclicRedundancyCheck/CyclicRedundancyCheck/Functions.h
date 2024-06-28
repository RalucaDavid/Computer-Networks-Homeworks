#pragma once
#include <regex>
#include <iostream>

void ReadData(std::string& inputMessage, std::string& generatorPolynomial)
{
	std::cout << "Introduce the input message:\n";
	std::cin >> inputMessage;
	std::cout << "Introduce the generating polynomial:\n";
	std::cin >> generatorPolynomial;
}

bool VerifyInput(const std::string& inputMessage, const std::string& generatorPolynomial)
{
	std::regex pattern("^[01]+$");
	if ((std::regex_match(inputMessage, pattern)) && (std::regex_match(generatorPolynomial, pattern)) 
		&& (generatorPolynomial[0]=='1')
		&& (inputMessage.size()>generatorPolynomial.size()))
		return true;
	std::cout << "One of the inputs is not valid.\n";
	return false;
}

void AddZeros(std::string& inputMessage, int grad)
{
	while (grad)
	{
		inputMessage.push_back('0');
		grad--;
	}
	std::cout << "The extended message is: "<<inputMessage<<"\n";
}

void EliminateZeros(std::string& inputMessage)
{
	while (inputMessage[0] == '0')
	{
		inputMessage.erase(0, 1);
	}
}

std::string XorOperation(std::string inputMessage,const  std::string& generatorPolynomial)
{
	while (inputMessage.size() >= generatorPolynomial.size())
	{
		EliminateZeros(inputMessage);
		if (inputMessage.size() >= generatorPolynomial.size())
		{
			std::cout << "XOR calculation:\n";
			std::cout << inputMessage << "\n" << generatorPolynomial << "\n";
			for (int index = 0; index < generatorPolynomial.size(); index++)
			{
				if (inputMessage[index] == generatorPolynomial[index])
					inputMessage[index] = '0';
				else
					inputMessage[index] = '1';
			}
			std::cout << inputMessage << "\n";
		}
	}
	if (inputMessage.empty())
		return "0";
	return inputMessage;
}

void AddRest(std::string inputMessage, const std::string& rest)
{
	for (int index = 0; index < rest.size(); index++)
	{
		if (inputMessage[inputMessage.size() - index-1]==rest[rest.size()-index-1])
			inputMessage[inputMessage.size() - index-1]='0';
		else
			inputMessage[inputMessage.size() - index-1]='1';
	}
	std::cout << "The message sent is: " << inputMessage << "\n";
}