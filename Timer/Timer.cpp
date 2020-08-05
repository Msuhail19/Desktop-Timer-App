// Timer.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Timer.h"
#include <string>
#include <future>
#include <CommCtrl.h>
#include <Uxtheme.h>
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "uxtheme.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND mainWnd;									// main window
HWND listview;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);

HWND				getListView(HWND);
BOOL				InitListViewColumns();
BOOL				InsertListViewItems(HWND);
BOOL				AddListViewItem();

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    AddItem(HWND, UINT, WPARAM, LPARAM);
void				HandleWM_NOTIFY(LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);



    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TIMER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	// Initialise ListView and add columns
	listview = getListView(mainWnd);
	if (listview) {
		// add columns, set theme and show
		InitListViewColumns();

		if (InsertListViewItems(listview)) {
			
		}

		ShowWindow(listview, nCmdShow);
		UpdateWindow(listview);
	}
	else { return FALSE;  }
	
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TIMER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TIMER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TIMER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   mainWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU,
      CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

   if (!mainWnd)
   {
      return FALSE;
   }

   HWND hwndButton = CreateWindowEx(0,
	   L"BUTTON",L"+", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 730, 10, 30,
	   30, mainWnd, (HMENU)BTN_ADD, hInst, NULL);
	
   HWND hwndButton2 = CreateWindowEx(0,
	   L"BUTTON", L"-", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 730, 50, 30,
	   30, mainWnd, (HMENU)BTN_DEL, hInst, NULL);



   ShowWindow(mainWnd, nCmdShow);
   UpdateWindow(mainWnd);

   return TRUE;
}

HWND getListView(HWND parent) {
	// Initialise common controls
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	HWND hWndListView = CreateWindow(WC_LISTVIEW, L"ListView", WS_CHILD | WS_BORDER | LVS_REPORT | LVS_EDITLABELS, 10, 10, 700, 500, parent, NULL, hInst, NULL);

	SendMessage(hWndListView, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	return hWndListView;
}


// Call to intialise headings in listview
BOOL InitListViewColumns() {
	WCHAR szText[256];     // Temporary buffer.
	LVCOLUMN lvc;

	// Initialize the LVCOLUMN structure.
	// The mask specifies that the format, width, text,
	// and subitem members of the structure are valid.
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	int colSize[C_COLUMNS] = { 50, 175, 175,175,125 };

	// Add the columns.
	for (int iCol = 0; iCol < C_COLUMNS; iCol++)
	{
		lvc.iSubItem = iCol;
		lvc.pszText = szText;
		lvc.fmt = LVCFMT_CENTER;	// center alignement
		lvc.cx = colSize[iCol];

		// Load the names of the column headings from the string resources IN RC FILE
		LoadString(hInst, IDS_FIRSTCOLUMN + iCol, szText, sizeof(szText) / sizeof(szText[0]));

		// Insert the columns into the list view.
		if (ListView_InsertColumn(listview, iCol, &lvc) == -1)
			return FALSE;
	}

	return TRUE;
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case BTN_ADD:
				//TODO: Complete
				// Add dialog to create button.
				DialogBox(hInst, MAKEINTRESOURCE(ID_ADDTIMER), hWnd, AddItem);
				break;
			case BTN_DEL:
				//TODO: Complete
				//Code to delete selected option.
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code)
		{
		case LVN_GETDISPINFO:
		{
			NMLVDISPINFO* plvdi = (NMLVDISPINFO*)lParam;
			plvdi = (NMLVDISPINFO*)lParam;

			switch (plvdi->item.iSubItem)
			{
			case 0: {
				std::wstring s = std::to_wstring(timerInfo[plvdi->item.iItem].ID);
				wchar_t* id = _wcsdup(s.c_str());
				plvdi->item.pszText = id;
				break;
			}

			case 1: {
				LPWSTR time = new wchar_t[20];
				GetTimeFormatEx(nullptr, 0, &timerInfo[plvdi->item.iItem].DateTime, L"HH':'mm':'ss", time, 20);
				plvdi->item.pszText = time;
				break;
			}

			case 2: {
				std::wstring s = std::to_wstring(threadvec[plvdi->item.iItem].Act.getSeconds());
				wchar_t* sec = _wcsdup(s.c_str());
				plvdi->item.pszText = const_cast<LPWSTR>(sec);
				break;
			}

			case 3: {
				plvdi->item.pszText = const_cast<LPWSTR>(timerInfo[plvdi->item.iItem].Action);
				break;
			}

			case 4: {
				plvdi->item.pszText = const_cast<LPWSTR>(timerInfo[plvdi->item.iItem].ShowNotification);
				break;
			}

			default:
				break;
			}
			return TRUE;
		}
		}
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Function to insert items to ListView
BOOL InsertListViewItems(HWND hWndListView)
{
	int cItems = timerInfo.size();
	LVITEM lvI;
	lvI.pszText = LPSTR_TEXTCALLBACK;
	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvI.stateMask = 0;
	lvI.iSubItem = 0;
	lvI.state = 0;

	for (int index = 0; index < cItems; index++)
	{
		lvI.iItem = index;
		lvI.iImage = index;

		// Insert items into the list.
		
		if (ListView_InsertItem(listview, &lvI) == -1)
			return FALSE;
	}

	return TRUE;
}

BOOL AddListViewItem() {
	LVITEM lvI;
	lvI.pszText = LPSTR_TEXTCALLBACK;
	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvI.stateMask = 0;
	lvI.iSubItem = 0;
	lvI.state = 0;
	
	int index = timerInfo.size() - 1;
	lvI.iItem = index;
	lvI.iImage = index;

	try {
		if (ListView_InsertItem(listview, &lvI) == -1)
			return FALSE;
	}
	catch (std::exception& e) {
	
	}

	return TRUE;
}


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL || LOWORD(wParam) == IDCLOSE)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// Message handler for add item box.
INT_PTR CALLBACK AddItem(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	HWND combo = GetDlgItem(hDlg, IDC_COMBO1);
	HWND check = GetDlgItem(hDlg, IDC_CHECK1);
	HWND dt = GetDlgItem(hDlg, IDC_DATETIMEPICKER1);

	switch (message)
	{
	case WM_INITDIALOG:
		// Initialise dropdown menu
		for (auto action : Actions) {
			SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)action);
		}
		// Set notify as default
		SendMessage(combo, CB_SETCURSEL, 0, 0);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		// New switch, check if command is following.
		switch (LOWORD(wParam))
		{
			case IDCANCEL:
			case IDCLOSE: {
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			case ID_APPLY: {
				// Determine values of new timerinfo item
				int id = (timerInfo.size() != 0) ? (int)timerInfo[timerInfo.size() - 1].ID + 1 : 0;
				int selected_dropdown = SendMessage(combo, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				const wchar_t* notify = (SendMessage(check, BM_GETCHECK, (WPARAM)0, (LPARAM)0)) ? L"Y" : L"N";

				SYSTEMTIME time_selected;
				HRESULT success = SendMessage(dt, DTM_GETSYSTEMTIME, (WPARAM)0, (LPARAM)&time_selected);
				if (success == S_OK) {
					// Declare action variable, and begin thread countdown after placing both in global vector
					Action a{listview, id, time_selected, selected_dropdown, notify };
					threadvec.push_back({ a, new std::thread(&Action::beginCountdown,a) });

					timerInfo.push_back({ id, time_selected, Actions[selected_dropdown], notify });
					AddListViewItem();
				}
			
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
		}
		break;
	}
	return (INT_PTR)FALSE;
}