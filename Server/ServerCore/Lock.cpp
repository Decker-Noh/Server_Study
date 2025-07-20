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
