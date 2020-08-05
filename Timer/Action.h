#pragma once
#include <windows.h>
#include <thread>

class Action
{
private :
	HWND hwnd;
	int id;
	int ActionID;
	_int64 seconds;
	bool active = true;
	const wchar_t* ShowNotification;

public :
	Action(HWND hwnd, int ID, SYSTEMTIME time, int ActionID, const wchar_t* ShowNotification);
	~Action();
	int getAction();
	_int64 getSeconds();
	const wchar_t* getID();
	const wchar_t* getNotification();
	void beginCountdown();
	void setActive(bool active);
	void doAction();
};

