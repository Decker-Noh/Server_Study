#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include<future>
#include<Windows.h>
#include<ThreadManager.h>
#include <vector>

atomic<int32> sum;

void Calculator(int val)
{
	if (val == 1)
		return;
	if (val == 2 || val == 3)
	{
		sum++;
		return;
	}
	for (int i = 2; i < val; i++)
	{
		if ((val % i) == 0)
			return;
	}
	sum++;
}
void Machin(int start, int end)
{
	for (int i = start; i <= end; i++)
	{
		Calculator(i);
	}
}
int main()
{
	const int MAX_NUMBER = 100'0000;

	vector<thread> threads;
	int coreNum = std::thread::hardware_concurrency();
	
	int coreWork = (MAX_NUMBER / coreNum) + 1;

	for (int i = 0; i < coreNum; i++)
	{
		threads.push_back(thread(Machin, (i * coreWork) + 1, min(MAX_NUMBER,((i + 1) * coreWork))));

	}
	for (int i = 0; i < coreNum; i++)
	{
		threads[i].join();

	}
	cout << sum << endl;

}