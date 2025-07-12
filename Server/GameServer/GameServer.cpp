#include "pch.h"
#include <iostream>

#include <thread>
#include <atomic>
#include <mutex>

int32 sum = 0;
int32 cnt = 0;
class SpinLock
{
public:
	void Lock()
	{
		bool expected = false;
		bool desired = true;
		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;
			this_thread::sleep_for(1ms); //추가된부분
			cnt++;
		}
	}
	void UnLock()
	{
		_locked = false;
	}
private:
	atomic<bool> _locked = false;

};
SpinLock spinLock;
void Add()
{
	for (int32 i = 0; i < 100'0000; i++)
	{
		spinLock.Lock();
		sum++;
		spinLock.UnLock();
	}
};
void Sub()
{
	for (int32 i = 0; i < 100'0000; i++)
	{
		spinLock.Lock();
		sum--;
		spinLock.UnLock();
	}
};
int main()
{
	thread t1(Add);
	thread t2(Sub);

	t1.join();
	t2.join();
	cout << sum << endl;
	cout << cnt << endl;

}