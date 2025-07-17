#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include<future>
#include<Windows.h>
#include "ConcurrentQueue.h";
#include "ConcurrentStack.h";

LockStack<int32> s;
LockQueue<int32> q;

void Push()
{
	while (true)
	{
		int32 value = rand() % 100;
		q.Push(value);

		this_thread::sleep_for(10ms);
	}
}
void Pop()
{
	while (true)
	{
		int32 data = 0;
		if (q.TryPop(OUT data))
		{
			cout << data << endl;
		}
	}
}
int main()
{
	thread t1(Push);
	thread t2(Pop);

	t1.join();
	t2.join();
}