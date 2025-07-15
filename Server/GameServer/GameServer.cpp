#include "pch.h"
#include <iostream>

#include <thread>
#include <atomic>
#include <mutex>

#include<future>
int64 Calculate()
{
	int64 sum = 0;
	for (int32 i = 0; i < 1'00000'0000; i++)
		sum += i;
	return sum;
}
int main()
{
	std::future<int64> future = std::async(std::launch::async, Calculate);
	//2가지 옵션이 있음
	//deffered -> 싱글스레드로 단순 함수의 동작을 나중으로 미뤄서 하는것
	//async -> 스레드를 하나 생성해서 함수를 동작시킴
	cout << "ㅎㅇ" << endl;
	int64 result = future.get();
	cout << "ㅎㅇ2" << endl;
	cout << result << endl;

}