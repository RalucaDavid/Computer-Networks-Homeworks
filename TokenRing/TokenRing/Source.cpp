#include <iostream>
#include "Functions.h"

int main()
{
	std::array<Computer, 10> computers;
	Token token;
	bool directionClock;
    ComputerInitialization(computers);
	directionClock = ChooseDirection();
	CrossingComputers(token, computers, directionClock);
	return 0;
}