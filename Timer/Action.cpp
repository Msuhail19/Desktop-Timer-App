#include "Action.h"
#define _SECOND ((_int64) 10000000)
#define _MINUTE (60 * _SECOND)
#define _HOUR   (60 * _MINUTE)
#define _DAY    (24 * _HOUR)
#include <string>


Action::Action(HWND hwnd, int ID, SYSTEMTIME time, int actionID, const wchar_t* ShowNotification) {
	// Constructor
	this->hwnd = hwnd;
	this->id = ID;
	this->ActionID = actionID;
	this->ShowNotification = ShowNotification;

	// Get current time
	SYSTEMTIME current;
	GetLocalTime(&current);

	// Convert to filetime for arithmetic
	FILETIME current_ftime;
	FILETIME alarm_ftime;
	SystemTimeToFileTime(&current, &current_ftime);
	SystemTimeToFileTime(&time, &alarm_ftime);

	// Convert to quadword and then perform arithmetic
	ULONGLONG currentResult = (((ULONGLONG)current_ftime.dwHighDateTime) << 32) + current_ftime.dwLowDateTime;
	ULONGLONG alarmResult = (((ULONGLONG)alarm_ftime.dwHighDateTime) << 32) + alarm_ftime.dwLowDateTime;

	_int64 value = alarmResult - currentResult;
	_int64 sec = value;
	if (value < 0) {
		// Increase by a day because alarm is in past.
		sec = _DAY - sec;
	}
	sec = value / _SECOND;
	this->seconds = sec;
}

Action::~Action() {
	// Destructor
}

void Action::beginCountdown() {
	// Thread enters here 
	while (seconds > 0 && active) {
		std::this_thread::sleep_for(std::chrono::seconds(5));
		seconds = seconds - 5;
	}
	if (active) {
		// Perform action.
		doAction();
	}
}

void Action::doAction() {
	// if notify
	MessageBoxW(hwnd, (LPCWSTR)L"Timer Complete ! ", (LPCWSTR)L"Done!", MB_ICONWARNING);
	if (ActionID == 0) {
		
	}
	else if(ActionID == 1) {
	
	}
	else if (ActionID == 2) {
		
	}
	// if restart
}

const wchar_t* Action::getID() {
	std::wstring s = std::to_wstring(id);
	const wchar_t* id = _wcsdup(s.c_str());
	return id;
}

int Action::getAction() {
	return ActionID;
}

_int64 Action::getSeconds() {
	return seconds;
}

const wchar_t* Action::getNotification() {
	return ShowNotification;
}

void Action::setActive(bool active) {
	this->active = active;
}