#include "pch.h"
#include "DeadLockProfiler.h"

void DeadLockProfiler::PushLock(const char* name)
{
	//���� ���� �����Ǿ�����
	LockGuard guard(_lock);
	//���̵� ã�ų� �߱��Ѵ�.
	int32 lockId = 0;
	auto findIt = _nameToId.find(name);
	if (findIt == _nameToId.end()) //�����̳� �ȿ� ������ ���ι߱�
	{
		lockId = static_cast<int32>(_nameToId.size());
		_nameToId[name] = lockId;
		_idToName[lockId] = name;
	}
	else //�����̳ʿ� ������ lockId�� �Ҵ�
	{
		lockId = findIt->second;
	}

	//����ִ� ���� �ִٸ�?

	if (_lockStack.empty() == false) //lockStack�� ���� �ִٸ�
	{
		//������ �߰ߵ��� ���� ���̽���� ����� ���θ� �ٽ� Ȯ��
		const int32 prevId = _lockStack.top();
		if (lockId != prevId) //���� �ֱ��� ���� �ƴ϶��
		{
			set<int32>& history = _lockHistory[prevId];//���� �����丮 ����
			if (history.find(lockId) == history.end())//�����丮�� ���ο� ���� ���ٸ�
			{
				history.insert(lockId); //�����丮�� ����.
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
	if (_discoverOrder[here] != -1)//�湮������ ������ ����
	{
		return;
	}
	_discoverOrder[here] = _discoverCount++;

	//��� ������ ������ ��ȸ�ؾ���.

	auto findIt = _lockHistory.find(here);

	if (findIt == _lockHistory.end())//�ش������� ����ä�� �ٸ��� ������� ����
	{
		_finished[here] = true;
		return;
	}

	set<int32>& nextSet = findIt->second;

	for (int32 there : nextSet)
	{
		//���� �湮������ ���ٸ� �湮�Ѵ�.
		if (_discoverOrder[there] == -1)
		{
			_parent[there] = here;
			Dfs(there);
			continue;
		}
		//here�� there���� ���� �߰ߵǾ��ٸ�, there�� here�� �ļ��̴�(������)
		if (_discoverOrder[here] < _discoverOrder[there])
		{
			continue;
		}
		//�ݴ�(������ ����)
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
