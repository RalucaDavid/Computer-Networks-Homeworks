#include "Functions.h"

int main()
{
	std::string inputMessage, generatorPolynomial, rest;
	ReadData(inputMessage, generatorPolynomial);
	if (VerifyInput(inputMessage, generatorPolynomial))
	{
		AddZeros(inputMessage, generatorPolynomial.size() - 1);
		rest = XorOperation(inputMessage, generatorPolynomial);
		std::cout << "The rest is:" << rest << "\n";
		AddRest(inputMessage, rest);
	}
	return 0;
}