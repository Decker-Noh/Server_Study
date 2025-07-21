#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"

void Lock::WriteLock()
{
	//아무도 소유 및 공유하고 있지 않을때 경합권을 가진다.
	
	//같은 스레드라면 재귀를 위해 통과를 허용
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;

	if (lockThreadId == LthreadId)
	{
		_writeCount++;
		return;
	}

	const int64 beginTick = ::GetTickCount64();

	//THREADID를 왼쪽으로 16비트만큼 이동 후 하위 16비트는 0으로 유지
	const uint32 desired = ((LthreadId << 16) & WRITE_THREAD_MASK);
	
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = EMPTY_FALG;
			if (_lockFlag.compare_exchange_strong(expected, desired))
			{
				_writeCount++;
				return;
			}
		}

		if (::GetTickCount64() - beginTick > AQUIRE_TIMEOUT_TICK)
			CRASH("Lock TIMEOUT");
		this_thread::yield(); //5천번 시도후 잠자기
	}
}

void Lock::WriteUnlock()
{

	//ReadLock을 다 풀기전까지는 WriteLock 불가능
	//WriteLock이 잡혀있는데 Read 카운트가 있으면 뭔가 이상하니 크래쉬
	if ((_lockFlag.load() & READ_THREAD_MASK) != 0)
		CRASH("INVALID_UNLOCK_ORDER");
	const int32 lockCount = --_writeCount;
	if (lockCount == 0)
	{
		_lockFlag.store(EMPTY_FALG);
	}
}

void Lock::ReadLock()
{
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;

	if (lockThreadId == LthreadId)
	{
		_lockFlag.fetch_add(1);
		return;
	}

	const int64 beginTick = ::GetTickCount64();
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = (_lockFlag.load() & READ_THREAD_MASK); //write를 사용중이어도 접근 가능
			if (_lockFlag.compare_exchange_strong(expected, expected + 1))
			{
				return;
			}
		}

		if (::GetTickCount64() - beginTick > AQUIRE_TIMEOUT_TICK)
			CRASH("Lock TIMEOUT");
		this_thread::yield();
	}
}

void Lock::ReadUnlock()
{
	if ((_lockFlag.fetch_sub(1) & READ_THREAD_MASK) == 0)
	{
		CRASH("MULTIPLE_UNLOCK");
	}
}

