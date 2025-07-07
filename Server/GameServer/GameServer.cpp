#include "pch.h"
#include <iostream>

#include <thread>

void HelloThread()
{
    cout << "Hello Thread" << endl;
}
int main()
{

    std::thread t;

    t = std::thread(HelloThread);
    cout << "Hello Main" << endl;


    int32 count = t.hardware_concurrency(); //가용 가능한 코어 갯수
    
    cout << count << endl;

    t.join(); //쓰레드 작업이 끝날때까지 부모 쓰레드 대기
}