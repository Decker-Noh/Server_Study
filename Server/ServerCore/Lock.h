#pragma once

#include "Types.h"

/*
[WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
W : WriteFlag (Exclusive Lock Owner Thread
R : ReadFlag (Shared Lock Count)

W -> R (o) ���� ������ �б�� ��
R -> W (x) ���� �� ���� �ȵ�
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
	void WriteLock(const char* name);
	void WriteUnlock(const char* name);
	void ReadLock(const char* name);
	void ReadUnlock(const char* name);
private:
	Atomic<uint32> _lockFlag = EMPTY_FALG; //���� ���� ���� 32��Ʈ
	uint16 _writeCount; //��͸� ��������� �����ϱ� ���� ����, ������ �����尡 ����ִٸ� ���
};

/*WR Lock Guard*/

class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock, const char* name) :_lock(lock), _name(name) { _lock.ReadLock(name); }
	~ReadLockGuard() { _lock.ReadUnlock(_name); }
private:
	Lock& _lock;
	const char* _name;

};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock, const char* name) :_lock(lock), _name(name) { _lock.WriteLock(name); }
	~WriteLockGuard() { _lock.WriteUnlock(_name); }
private:
	Lock& _lock;
	const char* _name;

};