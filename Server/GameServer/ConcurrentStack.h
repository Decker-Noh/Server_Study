#pragma once

#include <mutex>

template<typename T>
class LockStack
{
public:
	LockStack () {}
	LockStack(const LockStack&) = delete;
	LockStack& operator=(const LockStack&) = delete;

	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_stack.push(std::move(value));
		_conVar.notify_one();
		
	}
	bool Pop(T& value)
	{
		lock_guard<mutex> lock(_mutex);
		if (_stack.empty())
			return false;
		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}
	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		_conVar.wait(lock, [this] { return _stack.empty() == false; });
		value = std::move(_stack.top());
		value = stack.top();
		_stack.pop();
	}
private:
	stack<T> _stack;
	mutex _mutex;
	condition_variable _conVar;
};
