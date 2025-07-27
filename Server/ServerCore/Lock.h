#pragma once

#include "Types.h"

/*
[WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
W : WriteFlag (Exclusive Lock Owner Thread
R : ReadFlag (Shared Lock Count)

W -> R (o) 쓰고 있을때 읽기는 됨
R -> W (x) 읽을 때 쓰면 안됨
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
	Atomic<uint32> _lockFlag = EMPTY_FALG; //현재 락의 상태 32비트
	uint16 _writeCount; //재귀를 허락하지만 추적하기 위한 변수, 동일한 스레드가 잡고있다면 허락
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