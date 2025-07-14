#include "pch.h"
#include <iostream>

#include <thread>
#include <atomic>
#include <mutex>
#include<Windows.h>

mutex m;
queue<int32> q;

HANDLE handle;

void Producer()
{
	while (true)
	{
		{
			unique_lock<mutex> lock(m);

			q.push(100);
		}

		::SetEvent(handle); //파란불로 바꿔라
		this_thread::sleep_for(1000000ms);
	}
	
};
void Consumer()
{
	while (true)
	{
		WaitForSingleObject(handle, INFINITE);//파란불(true) 될때까지 계속 기다려라
		unique_lock<mutex> lock(m);

		if (q.empty() == false)
		{
			int32 data = q.front();
			q.pop();
			cout << data << endl;
		}
	
	}
};

int main()
{
	handle = CreateEvent(NULL/*보안*/, FALSE/*MaunualReset*/, FALSE, NULL);
	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();
}