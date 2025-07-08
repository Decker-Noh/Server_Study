#include "pch.h"
#include <iostream>

#include <thread>
#include <atomic>
void HelloThread()
{
    cout << "Hello Thread" << endl;
}
atomic<int32> sum = 0;
void Add()
{
    for (int32 i = 0; i < 100'000; i++)
    {
        sum++;
    }
}
void Sub()
{
    for (int32 i = 0; i < 100'000; i++)
    {
        sum--;
    }
}
int main()
{
    thread t1(Add);
    thread t2(Sub);

    t1.join();
    t2.join();
    
    cout << sum << endl;

}