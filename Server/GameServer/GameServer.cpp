#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include<future>
#include<Windows.h>
#include<ThreadManager.h>

#include "AccountManager.h"
#include "PlayerManager.h"


int main()
{
	GThreadManager->Launch([=]
		{
			while (true)
			{
				cout << "Player Then Account" << endl;
				GPlayerManager.PlayerThenAccount();
				this_thread::sleep_for(100ms);
			}
		});
	GThreadManager->Launch([=]
		{
			while (true)
			{
				cout << "Account Then Player" << endl;
				GAccountManager.AccountThenPlayer();
				this_thread::sleep_for(100ms);
			}
		});
	GThreadManager->Join();
}