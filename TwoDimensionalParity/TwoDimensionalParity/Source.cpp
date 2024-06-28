#include <iostream>
#include "Functions.h"

int main()
{
	std::string inputMessage;
	std::vector<std::vector<bool>> matrix;
	std::cout << "Introduce the input message:\n";
	std::cin >> inputMessage;
	if (CheckMessage(inputMessage))
	{
		MatrixConstruction(inputMessage, matrix);
		Corruption(matrix);
		DestinationCheck(matrix);
	}
	return 0;
}