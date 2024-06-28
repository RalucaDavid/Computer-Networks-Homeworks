#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <unordered_set>
#include <random>

std::mutex mtx;

void ShowDestination(const std::vector<int>& destinationMemory)
{
	std::cout << "[Destination]: ";
	for (int index = 0; index < destinationMemory.size(); index++)
		std::cout << destinationMemory[index] << " ";
	std::cout << "\n";
}

// For tests
std::unordered_set<int> lostACK,lostTransmissions;

void GenerateRandom(int number)
{
	std::random_device rd;
	std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> distrib(1, number);
	int number1 = 2;
	while (number1 > 0)
	{
		lostACK.insert(distrib(gen));
		lostTransmissions.insert(distrib(gen));
		number1--;
	}
}

void TransmitElement(int index, const std::vector<int>& sourceMemory, std::vector<int>& destinationMemory, std::vector<bool>& ackReceived, std::atomic<bool>& timeout, std::vector<std::atomic<bool>>& elementSent)
{
	auto start = std::chrono::steady_clock::now();
	while ((!timeout)&&(!ackReceived[index]))
	{
		{
			std::lock_guard<std::mutex> lock(mtx);
			std::cout << "[Source]: Transmitting element " << sourceMemory[index] << ".\n";
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		{
			std::lock_guard<std::mutex> lock(mtx);
			if (lostTransmissions.find(index)==lostTransmissions.end())
			{
				if (!elementSent[index])
				{
					elementSent[index] = true;
					destinationMemory.push_back(sourceMemory[index]);
				}
			}
			else
			{
				lostTransmissions.erase(index);
			}
			if (elementSent[index])
			{
				std::cout << "[Destination]: Received element " << sourceMemory[index] << ". Transmitting ACKnowledged.\n";
				ShowDestination(destinationMemory);
				if (lostACK.find(index)==lostACK.end())
				{
					ackReceived[index] = true;
				}
				else
				{
					lostACK.erase(index);
				}
				if (ackReceived[index])
				{
					std::cout << "[Source]: ACKnowledged for the element " << sourceMemory[index] << ".\n";
				}
			}
		}
		auto end = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		if ((duration.count() > 2000) || (!elementSent[index]) || (!ackReceived[index]))
		{
			std::cout << "[Source]: Timeout occurred. The element " << sourceMemory[index] << " must be submitted.\n";
		}
		break;
	}
}

int main()
{
	std::vector<int> sourceMemory = { 1, 2, 3, 4, 5, 6 }, destinationMemory;
	int windowSize = 3;
	std::vector<bool> ackReceived(sourceMemory.size(), false);
	std::vector<std::atomic<bool>> elementSent(sourceMemory.size());
	std::atomic<bool> timeout(false);

	int startWindow = 0;
	int endWindow = windowSize - 1;
	GenerateRandom(sourceMemory.size());
	while (startWindow < sourceMemory.size())
	{
		std::vector<std::thread> threads;
		for (int index = startWindow; index <= endWindow && index < sourceMemory.size(); ++index)
		{
			threads.emplace_back(TransmitElement, index, std::cref(sourceMemory), std::ref(destinationMemory), std::ref(ackReceived), std::ref(timeout), std::ref(elementSent));
		}

		for (auto& thread : threads)
		{
			thread.join();
		}

		for (int index = startWindow; index <= endWindow && index < sourceMemory.size(); index++)
		{
			if (ackReceived[index])
			{
				startWindow++;
				endWindow++;
			}
			else
				break;
		}
	}

	//Sortation
	std::vector<std::pair<int, int>> indexedDestinationMemory;
	for (int index = 0; index < destinationMemory.size(); index++) 
	{
		auto it = std::find(sourceMemory.begin(), sourceMemory.end(), destinationMemory[index]);
		if (it != sourceMemory.end()) 
		{
			int position = std::distance(sourceMemory.begin(), it);
			indexedDestinationMemory.emplace_back(destinationMemory[index], position);
		}
	}
	std::sort(indexedDestinationMemory.begin(), indexedDestinationMemory.end(),
		[&](const auto& lhs, const auto& rhs) 
		{
			return sourceMemory[lhs.second] < sourceMemory[rhs.second];
		});
	for (int index = 0; index < indexedDestinationMemory.size(); index++) 
	{
		destinationMemory[index] = indexedDestinationMemory[index].first;
	}
	std::cout << "\nFinal result:\n";
	ShowDestination(destinationMemory);
	return 0;
}
