#include "pch.h"
#include <iostream>

#include <thread>
#include <atomic>
#include <mutex>

class Task1
{
public:
	static Task1* Instance()
	{
		static Task1 instance;
		return &instance;
	}
	void GetTask(int32 n)
	{
		lock_guard<mutex> guard(mutex);
		//뭔갈함
	}
	void Save1()
	{
		lock_guard<mutex> guard(mutex);//Task1의 뮤텍스 락
		Task2::Instance()->GetTask(1); //Task2의 뮤텍스 락
	};
private:
	mutex _mutex;
};
class Task2
{
public:
	static Task2* Instance()
	{
		static Task2 instance;
		return &instance;
	}
	void GetTask(int32 n)
	{
		lock_guard<mutex> guard(mutex);
		//뭔갈함
	}
	void Save2()
	{
		lock_guard<mutex> guard(mutex);//Task2의 뮤텍스 락
		Task1::Instance()->GetTask(1);//Task1의 뮤텍스 락
	};
private:
	mutex _mutex;
};

int main()
{
	thread t1(Push);
	thread t2(Push);

	t1.join();
	t2.join();
	cout << v.size() << endl;

}