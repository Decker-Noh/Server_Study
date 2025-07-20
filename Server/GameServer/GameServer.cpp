﻿#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include<future>
#include<Windows.h>
#include<ThreadManager.h>


void ThreadMain()
{
	while (true)
	{
		cout << "Hello Thread Manager"<< LthreadId << endl;

		this_thread::sleep_for(1s);
	}
}
int main()
{
	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch(ThreadMain);
	}

	GThreadManager->Join();

}