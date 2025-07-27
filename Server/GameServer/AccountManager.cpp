#include "pch.h"
#include "AccountManager.h"
#include "PlayerManager.h"

AccountManager GAccountManager;
void AccountManager::AccountThenPlayer()
{
	WRITE_LOCK;
	this_thread::sleep_for(100ms);
	GPlayerManager.Lock();
}

void AccountManager::Lock()
{
	WRITE_LOCK;
}
