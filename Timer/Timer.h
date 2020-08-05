#pragma once

#include "Action.h"
#include "resource.h"
#include "framework.h"
#include "winuser.h"
#include <Windows.h>
#include <vector>
#include <memory>
#include <thread>
#define C_COLUMNS	5

const wchar_t* Actions[] = { L"Notify Only", L"Restart",L"ShutDown" };

struct TimerItem
{
	int ID;
	SYSTEMTIME DateTime;
	const wchar_t* Action;
	const wchar_t* ShowNotification;
};

struct ActionThreaded {
	Action Act;
	std::thread* thread;
};

std::vector<ActionThreaded> threadvec = {};

std::vector<TimerItem> timerInfo= {

};