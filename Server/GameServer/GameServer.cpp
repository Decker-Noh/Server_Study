#include "pch.h"
#include <iostream>

#include <thread>
#include <atomic>
#include <mutex>

mutex m;
queue<int32> q;

condition_variable cv;
void Producer()
{
	while (true)
	{
		{
			unique_lock<mutex> lock(m);

			q.push(100);
		}
		cv.notify_one();
		//락을 잡고
		//공유 변수 수정
		//락을 풀고
		//다른 쓰레드에게 통지
	}
	
};
void Consumer()
{
	while (true)
	{
		unique_lock<mutex> lock(m);
		cv.wait(lock, []() {return q.empty() == false;});
		// 1.락을 잡고
		// 2. 조건 확인 해주고
		// 3. 조건 맞으면 진행, 아니면 다시 대기 모드

		int32 data = q.front();
		q.pop();
		cout << q.size() << endl;

	
	}
};

int main()
{

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();
}