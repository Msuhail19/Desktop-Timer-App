#pragma once

#include "resource.h"
#include "framework.h"
#include "Timer.h"
#include "winuser.h"
#include <Windows.h>
#include <vector>
#define C_COLUMNS	4


struct TimerItem
{
	int ID;
	SYSTEMTIME DateTime;
	const wchar_t* Action;
	const wchar_t* ShowNotification;
};

std::vector<TimerItem> timerInfo= {
};