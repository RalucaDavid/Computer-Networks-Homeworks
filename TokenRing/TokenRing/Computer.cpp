#include "Computer.h"

Computer::Computer()
{
	/*empty*/
}

Computer::Computer(const std::string& IP) : IP(IP)
{
	/*empty*/
}

std::string Computer::GetIP() const
{
	return IP;
}

std::string Computer::GetBuffer() const
{
	return buffer;
}

void Computer::AddMessage(const std::string& message)
{
	this->buffer = this->buffer + "; " + message;
}
