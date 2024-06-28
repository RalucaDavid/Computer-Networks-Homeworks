#pragma once
#include <array>
#include <random>
#include <fstream>
#include "Computer.h"
#include "Token.h"

std::string GenerateRandomIPAddress()
{
    std::string ipAddress;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 255);
    for (int index = 0; index < 4; index++)
    {
        if (index > 0)
        {
            ipAddress += ".";
        }
        ipAddress += std::to_string(dis(gen));
    }
    return ipAddress;
}

int GenerateRandomPosition()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 9);
    return dis(gen);
}

void ComputerInitialization(std::array<Computer, 10>& computers)
{
    for (int index = 0; index < computers.size(); index++)
    {
        std::string randomIP = GenerateRandomIPAddress();
        computers[index] = Computer(randomIP);
    }
}

void SourceAndDestinationInitialization(Token& token,const std::array<Computer, 10>& computers)
{
    token.SetCalculatorSource(computers[GenerateRandomPosition()].GetIP());
    int randomPosition = GenerateRandomPosition();
    while (computers[randomPosition].GetIP() == token.GetCalculatorSource())
    {
        randomPosition = GenerateRandomPosition();
    }
    token.SetCalculatorDestination(computers[randomPosition].GetIP());
}

bool ChooseDirection()
{
    while (1)
    {
        std::string answer;
        std::cout << "Choose if you want the direction to be clockwise (Yes/No):\n";
        std::cin >> answer;
        if (answer == "Yes")
            return true;
        else if(answer=="No")
            return false;
        std::cout << "Invalid answer!\n";
    }
}

std::vector<std::string> ReadMessages()
{
    std::string message;
    std::vector<std::string> messages;
    std::ifstream fin("Input.txt");
    while (!fin.eof())
    {
        fin >> message;
        messages.push_back(message);
    }
    fin.close();
    return messages;
}

void ShowComputers(const std::array<Computer, 10>& computers)
{
    for (int index=0;index<computers.size();index++)
    {
        std::cout << "C" << index << "(" << computers[index].GetIP() << ") -> " << computers[index].GetBuffer()<<"\n";
    }
}

int FindComputer(const std::string& IP, const std::array<Computer, 10>& computers)
{
    for (int index = 0; index < computers.size(); index++)
        if (computers[index].GetIP() == IP)
            return index;
    return -1;
}

void CrossingComputers(Token& token, std::array<Computer, 10>& computers, const bool& directionClock)
{
    std::vector<std::string> messages = ReadMessages();
    int source, destination, lastLocation=-1,jndex;
    if (directionClock)
    {
        jndex = 1;
    }
    else
    {
        jndex = computers.size() - 1;
    }
    ShowComputers(computers);
    for (const auto& message : messages)
    {
        SourceAndDestinationInitialization(token, computers);
        source = FindComputer(token.GetCalculatorSource(), computers);
        destination = FindComputer(token.GetCalculatorDestination(), computers);
        std::cout << "Sursa: C" <<source<< " Destinatia: C" << destination << "\n";
        int index;

        // For the first step
        if (lastLocation == -1)
        {
            lastLocation = source;
        }

        // Go to source 
        for (index = lastLocation; index != source; index = (index + jndex) % computers.size())
        {
            std::cout << "C" << index << ": Muta jetonul.\n";
        }
        if (computers[index].GetIP() != computers[lastLocation].GetIP())
        {
            std::cout << "C" << index << ": Am preluat jetonul.\n";
        }
        std::cout << "C" << index << ": Muta jetonul.\n";
        token.SetMessage(message);
        token.SetFree(false);
        token.SetArrivedDestination(false);

        // Go to destination and back to source
        for (index = (source + jndex) % computers.size(); index != source; index = (index + jndex) % computers.size())
        {
            if (computers[index].GetIP()==token.GetCalculatorDestination())
            {
                std::cout << "C" << index << ": Am ajuns la destinatie.\n";
                token.SetArrivedDestination(true);
                computers[index].AddMessage(message);
            }
            std::cout << "C" << index << ": Muta jetonul." << "\n";
        }
        std::cout << "C" << index << ": Am ajuns inapoi.\n";
        token.SetFree(true);
        token.SetMessage("");
        token.SetCalculatorSource("");
        token.SetCalculatorDestination("");
        lastLocation = source;
        ShowComputers(computers);
    }
}