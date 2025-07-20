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
	}
}

void Lock::WriteUnlock()
{
}

void Lock::ReadLock()
{
}

void Lock::ReadUnlock()
{
}
