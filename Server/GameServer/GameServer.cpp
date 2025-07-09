#include "pch.h"
#include <iostream>

#include <thread>
#include <atomic>
#include <mutex>

vector<int32> v;

mutex m;

template<typename T>
class LockGuard
{
public:
	LockGuard(T& m)
	{
		_mutex = &m;
		_mutex->lock();
	}
	~LockGuard()
	{
		_mutex = &m;
		_mutex->unlock();
	}
private:
	T* _mutex;
};

void Push()
{
	for (int32 i = 0; i < 10000; i++)
	{
		LockGuard<std::mutex> lockGuard(m);
		v.push_back(1);

	}
}

int main()
{
	thread t1(Push);
	thread t2(Push);

	t1.join();
	t2.join();
	cout << v.size() << endl;

}