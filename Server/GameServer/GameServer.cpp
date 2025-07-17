#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include<future>



thread_local int32 LTreadID = 0;

void ThreadMain(int32 _threadID)
{
	while (true)
	{
		LTreadID = _threadID;
		cout << "Hello Thread!" << LTreadID << endl;
		this_thread::sleep_for(1s);
	}
}
int main()
{
	vector<thread> threads;
	for (int32 i = 0; i < 10; i++)
	{
		int32 threadID = i + 1;
		
		threads.push_back(thread(ThreadMain, threadID));
	}
	for (thread& t : threads)
		t.join();
}