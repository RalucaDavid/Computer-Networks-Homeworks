#pragma once
#include <string>
#include <regex>
#include <vector>
#include <random>

bool CheckMessage(std::string inputMessage)
{
	std::regex pattern("^[01]+$");
	if ((inputMessage.size() % 7 == 0) && (std::regex_match(inputMessage, pattern)))
		return true;
	std::cout << "The message is not valid.\n";
	return false;
}

void ShowMatrix(const std::vector<std::vector<bool>>& matrix)
{
	const int numberLines = matrix.size();
	const int numberColumns = 8;
	for (int index = 0; index < numberLines; index++)
	{
		for (int jndex = 0; jndex < numberColumns; jndex++)
		{
			std::cout << matrix[index][jndex];
		}
		std::cout << "\n";
	}
}

void MatrixConstruction(std::string inputMessage, std::vector<std::vector<bool>>& matrix)
{
	int const numberLines = inputMessage.size() / 7;
	int const numberColumns = 7;
	int numberOnes = 0;
	for (int index = 0; index < numberLines; index++)
	{
		std::vector<bool> line;
		for (int jndex = 0; jndex < numberColumns; jndex++)
		{
			if (inputMessage[7 * index + jndex] == '1')
				numberOnes++;
			line.push_back(inputMessage[7 * index + jndex] == '1');
		}
		line.push_back(numberOnes % 2 == 1); // for the last/parity column
		matrix.push_back(line);
	}
	std::vector<bool> lineParity; // for the last/parity line 
	for (int jndex = 0; jndex < numberColumns+1; jndex++)
	{
		numberOnes = 0;
		for (int index = 0; index < numberLines; index++)
		{
			if (matrix[index][jndex] == 1)
				numberOnes++;
		}
		lineParity.push_back(numberOnes % 2 == 1);
	}
	matrix.push_back(lineParity);
	std::cout << "The constructed matrix is: \n";
	ShowMatrix(matrix);
}

std::pair<bool,bool> RandomPositionCorruption(const int& numberLines)
{
	std::random_device rd; 
	std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> line(0,numberLines-2);
	std::uniform_int_distribution<> column(0,6);
	return std::make_pair(line(gen), column(gen));
}

void Corruption(std::vector<std::vector<bool>>& matrix)
{
	std::pair<int, int> position = RandomPositionCorruption(matrix.size());
	matrix[position.first][position.second] = !matrix[position.first][position.second];
	std::cout << "The corrupted matrix is: \n";
	ShowMatrix(matrix);
}

void DestinationCheck(const std::vector<std::vector<bool>>& matrix)
{
	int const numberLines = matrix.size();
	int const numberColumns = 8;
	int numberOnes = 0;
	std::pair<int, int> position = { -1, -1 };
	for (int jndex = 0; jndex < numberColumns; jndex++)
	{
		numberOnes = 0;
		for (int index = 0; index < numberLines-1; index++)
		{
			if (matrix[index][jndex] == 1)
				numberOnes++;
		}
		if ((numberOnes % 2 == 1) != matrix[numberLines - 1][jndex])
		{
			position.second = jndex;
			break;
		}
	}
	for (int index = 0; index < numberLines; index++)
	{
		numberOnes = 0;
		for (int jndex = 0; jndex < numberColumns - 1; jndex++)
		{
			if (matrix[index][jndex] == 1)
				numberOnes++;
		}
		if ((numberOnes % 2 == 1) != matrix[index][numberColumns-1])
		{
			position.first = index;
			break;
		}
	}
	if (position.first != -1 && position.second != -1)
		std::cout << "The position of the corrupt bit is (" << position.first << ", " << position.second << ")";
	else
		std::cout << "No corrupt bit found.";
}