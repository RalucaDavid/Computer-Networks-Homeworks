#include "Token.h"

Token::Token()
{
	free = false;
	arrivedDestination = false;
}

void Token::SetCalculatorSource(const std::string& IP)
{
	calculatorSource = IP;
}

void Token::SetCalculatorDestination(const std::string& IP)
{
	calculatorDestination = IP;
}

void Token::SetMessage(const std::string& message)
{
	this->message = message;
}

void Token::SetFree(const bool& free)
{
	this->free = free;
}

void Token::SetArrivedDestination(const bool& arrivedDestination)
{
	this->arrivedDestination = arrivedDestination;
}

std::string Token::GetCalculatorSource() const
{
	return calculatorSource;
}

std::string Token::GetCalculatorDestination() const
{
	return calculatorDestination;
}
