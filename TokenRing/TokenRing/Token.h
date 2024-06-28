#pragma once
#include <string>

class Token
{
public:
	Token();
	void SetCalculatorSource(const std::string& IP);
	void SetCalculatorDestination(const std::string& IP);
	void SetMessage(const std::string& message);
	void SetFree(const bool& free);
	void SetArrivedDestination(const bool& arrivedDestination);
	std::string GetCalculatorSource() const;
	std::string GetCalculatorDestination() const;
private:
	std::string calculatorSource;
	std::string calculatorDestination;
	std::string message;
	bool free;
	bool arrivedDestination;
};

