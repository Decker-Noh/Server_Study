#include "pch.h"
#include "DeadLockProfiler.h"

void DeadLockProfiler::PushLock(const char* name)
{
	//락이 새로 생성되었을때
	LockGuard guard(_lock);
	//아이디를 찾거나 발급한다.
	int32 lockId = 0;
	auto findIt = _nameToId.find(name);
	if (findIt == _nameToId.end()) //컨테이너 안에 없으면 새로발급
	{
		lockId = static_cast<int32>(_nameToId.size());
		_nameToId[name] = lockId;
		_idToName[lockId] = name;
	}
	else //컨테이너에 있으면 lockId에 할당
	{
		lockId = findIt->second;
	}

	//잡고있는 락이 있다면?

	if (_lockStack.empty() == false) //lockStack에 값이 있다면
	{
		//기존에 발견되지 않은 케이스라면 데드락 여부를 다시 확인
		const int32 prevId = _lockStack.top();
		if (lockId != prevId) //제일 최근의 락이 아니라면
		{
			set<int32>& history = _lockHistory[prevId];//현재 히스토리 참조
			if (history.find(lockId) == history.end())//히스토리에 새로운 락이 없다면
			{
				history.insert(lockId); //히스토리에 넣음.
				CheckCycle();
			}
		}
	}
	_lockStack.push(lockId);
}

void DeadLockProfiler::PopLock(const char* name)
{
	LockGuard guard(_lock);

	if (_lockStack.empty())
	{
		CRASH("MULITPLE_UNLOCK");
	}
	int32 lockId = _nameToId[name];
	if (_lockStack.top() != lockId)
		CRASH("INVALID_UNLOCK");
	_lockStack.pop();
}

void DeadLockProfiler::CheckCycle()
{
	const int32 lockCount = static_cast<int32>(_nameToId.size());
	_discoverOrder = vector<int32>(lockCount, -1);
	_discoverCount = 0;
	_finished = vector<bool>(lockCount, false);
	_parent = vector<int32>(lockCount, -1);

	for (int32 lockId = 0; lockId < lockCount; lockId++)
	{
		Dfs(lockId);
	}
	_discoverOrder.clear();
	_finished.clear();
	_parent.clear();

}

void DeadLockProfiler::Dfs(int32 here)
{
	if (_discoverOrder[here] != -1)//방문한적이 있으면 리턴
	{
		return;
	}
	_discoverOrder[here] = _discoverCount++;

	//모든 인접한 정점을 순회해야함.

	auto findIt = _lockHistory.find(here);

	if (findIt == _lockHistory.end())//해당정점을 잡은채로 다른락 잡은기록 없음
	{
		_finished[here] = true;
		return;
	}

	set<int32>& nextSet = findIt->second;

	for (int32 there : nextSet)
	{
		//아직 방문한적이 없다면 방문한다.
		if (_discoverOrder[there] == -1)
		{
			_parent[there] = here;
			Dfs(there);
			continue;
		}
		//here가 there보다 먼저 발견되었다면, there는 here의 후손이다(순방향)
		if (_discoverOrder[here] < _discoverOrder[there])
		{
			continue;
		}
		//반대(역방향 간선)
		if (_finished[there] == false)
		{
			printf("%s -> %s\n", _idToName[here], _idToName[there]);
			int32 now = here;
			while (true)
			{
				printf("%s -> %s\n", _idToName[_parent[now]], _idToName[now]);
				now = _parent[now];
				if (now == there)
					break;
			}

			CRASH("DEADLOCK_DETECTED")
		}
	}
	_finished[here] = true;
}
