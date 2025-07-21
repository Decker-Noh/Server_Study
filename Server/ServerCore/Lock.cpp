#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"

void Lock::WriteLock()
{
	//�ƹ��� ���� �� �����ϰ� ���� ������ ���ձ��� ������.
	
	//���� �������� ��͸� ���� ����� ���
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;

	if (lockThreadId == LthreadId)
	{
		_writeCount++;
		return;
	}

	const int64 beginTick = ::GetTickCount64();

	//THREADID�� �������� 16��Ʈ��ŭ �̵� �� ���� 16��Ʈ�� 0���� ����
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
		this_thread::yield(); //5õ�� �õ��� ���ڱ�
	}
}

void Lock::WriteUnlock()
{

	//ReadLock�� �� Ǯ���������� WriteLock �Ұ���
	//WriteLock�� �����ִµ� Read ī��Ʈ�� ������ ���� �̻��ϴ� ũ����
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
			uint32 expected = (_lockFlag.load() & READ_THREAD_MASK); //write�� ������̾ ���� ����
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

