#pragma once

#include <map>
#include <vector>
#include <stack>


/*----------------------
	DeadLock Profiler
-----------------------*/
class DeadLockProfiler
{
public:
	void PushLock(const char* name);
	void PopLock(const char* name);
	void CheckCycle();
private:
	void Dfs(int32 index);
private:
	unordered_map<const char*, int32> _nameToId;
	unordered_map<int32, const char*> _idToName;
	stack<int32> _lockStack;
	map<int32, set<int32>> _lockHistory; 

	Mutex _lock;

private:
	vector<int32> _discoverOrder; //노드가 발견되는 순서를 기록한 배열
	int32 _discoverCount; //노드가 발견된 순서
	vector<bool> _finished; //Dfs[i]가 종료되었는지
	vector<int32> _parent; //나를 발견한 부모가 누구인지
};

