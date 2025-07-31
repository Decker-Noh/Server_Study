#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "RefCounting.h"

class Wraight : public RefCountable
{
public:
	int _hp = 150;
	int _posX = 0;
	int _posY = 0;
};

using WraightRef = TSharedPtr<Wraight>;

class Missile : public RefCountable
{
public:
	void SetTarget(WraightRef target)
	{
		_target = target;
		// 중간에 개입 가능
		//target->AddRef();
	}

	bool Update()
	{
		if (_target == nullptr)
			return true;

		int posX = _target->_posX;
		int posY = _target->_posY;

		// TODO : 쫓아간다

		if (_target->_hp == 0)
		{
			//_target->ReleaseRef();
			_target = nullptr;
			return true;
		}

		return false;
	}

	WraightRef _target = nullptr;
};

using MissileRef = TSharedPtr<Missile>;

int main()
{
	WraightRef wraight(new Wraight()); //wraight 참조 2
	wraight->ReleaseRef(); //wraight 참조 1
	MissileRef missile(new Missile()); //Missile 참조 2
	missile->ReleaseRef(); //Missile 참조 1

	missile->SetTarget(wraight); //wraight 참조 2

	// 레이스가 피격 당함
	wraight->_hp = 0;
	//delete wraight;
	//wraight->ReleaseRef();
	wraight = nullptr; //wraight 참조 1 //nullptr이 오른값처리되서 이동연산자가 발동됨

	while (true)
	{
		if (missile)
		{
			if (missile->Update())
			{
				//missile->ReleaseRef();
				missile = nullptr; //미사일 참조 0
			}
		}
	}

	//missile->ReleaseRef();
	missile = nullptr;
	//delete missile;
}