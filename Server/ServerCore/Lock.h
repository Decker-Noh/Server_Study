#pragma once

#include "Types.h"

/*
[WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
W : WriteFlag (Exclusive Lock Owner Thread
R : ReadFlag (Shared Lock Count)
*/
class Lock
{
	enum : uint32
	{
		AQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPIN_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_THREAD_MASK = 0x0000'FFFF,
		EMPTY_FALG = 0x0000'0000
	};
public:
	void WriteLock();
	void WriteUnlock();
	void ReadLock();
	void ReadUnlock();
private:
	Atomic<uint32> _lockFlag = EMPTY_FALG; //���� ���� ���� 32��Ʈ
	uint16 _writeCount; //��͸� ��������� �����ϱ� ���� ����, ������ �����尡 ����ִٸ� ���
};

