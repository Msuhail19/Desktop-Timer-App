#include "Action.h"

#include <string>

class Action {
	int id;
	const wchar_t* SelectedAction;
	const wchar_t* ShowNotification;


public:
	Action(int ID, SYSTEMTIME time, const wchar_t* ChosenAction, const wchar_t* ShowNotification) {
		// Constructor
		this->id = ID;
		this->SelectedAction = ChosenAction;
		this->ShowNotification = ShowNotification;
		SYSTEMTIME current;

	}

	~Action() {
		// Destructor

	}

	const wchar_t* getID() {
		std::wstring s = std::to_wstring(id);
		const wchar_t* id = _wcsdup(s.c_str());
		return id;
	}

	const wchar_t* getAction() {
		return SelectedAction;
	}

	const wchar_t* getNotification() {
		return ShowNotification;
	}
};
