#pragma once
#include <string>

class Computer
{
public:
	Computer();
	Computer(const std::string& IP);
	std::string GetIP() const;
	std::string GetBuffer() const;
	void AddMessage(const std::string& message);
private:
	std::string IP;
	std::string buffer = "null";
};

