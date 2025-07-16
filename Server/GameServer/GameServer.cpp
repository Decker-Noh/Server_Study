#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include<future>

int32 x = 0;
int32 y = 0;
int32 r1 = 0;
int32 r2 = 0;

int32 cnt = 0;
volatile bool ready = false;
void thread1()
{
	while (!ready)
		;
	x = 1;
	r2 = y;
}
void thread2()
{
	while (!ready)
		;
	y = 1;
	r1 = x;
}
int main()
{
	while (true)
	{

		x = y = r1 = r2 = 0;
		thread t1(thread1);
		thread t2(thread2);

		
		t1.join();
		t2.join();
		cnt++;
		if (r1 == 0 && r2 == 0)
			break;
	}
	cout << cnt << "번 걸림" << endl;
}