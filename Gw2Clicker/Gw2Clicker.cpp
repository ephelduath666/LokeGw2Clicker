// Gw2Clicker.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Gw2Clicker.h"
#include "Controls.h"
#include <sstream>
#include <windowsx.h>
#include "version.h"

#define MAX_LOADSTRING 100

// Global Variables:
DWORD WINAPI ClickerFunc(LPVOID lpParam);
HANDLE hClickerThread;
DWORD threadID = 0;

HINSTANCE hInst;                                // current instance
HWND hWndMain;                                  // Main window handler
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

TCHAR Profiles[3][30] =
{
    TEXT("Candy Corn Gobler"),
    TEXT("Snow Globe Gobler"),
    TEXT("Just get rid of the stack")
};

HWND tbMouseX, tbMouseY, cbProfile;
bool isClicking = FALSE;
int mouseX = 0, mouseY = 0;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

typedef struct ClickerInfo {
    int delay_in_ms = 1000;
    int mouse_x = 0;
    int mouse_y = 0;
    bool doubleClick = FALSE;
} CLICKER_INFO, *P_CLICKER_INFO;


CLICKER_INFO ciProfiles[3] = {
    {5200, 0, 0, TRUE},
    {100, 0, 0, TRUE},
    {50, 0, 0, TRUE}
};
// Stop clicking
//
void StopClicking() {
    isClicking = FALSE;
    Beep(750, 80);
    Beep(750, 80);
    DWORD ret;
    WaitForSingleObject(hClickerThread, 5000);
    hClickerThread = nullptr;
}

void StartClicking() {
    POINT p;
    int ix = ComboBox_GetCurSel(cbProfile);
    P_CLICKER_INFO currentClickerInfo = &ciProfiles[ix];
    if (GetCursorPos(&p)) {
        //std::wstringstream ssX, ssY;
        //ssX << p.x;
        //SendMessage(tbMouseX, WM_SETTEXT, 0, (LPARAM)ssX.str().c_str());
        //ssY << p.y;
        //SendMessage(tbMouseY, WM_SETTEXT, 0, (LPARAM)ssY.str().c_str());
        if (currentClickerInfo != nullptr) {
            currentClickerInfo->mouse_x = p.x;
            currentClickerInfo->mouse_y = p.y;
        }
        Beep(750, 100);
        mouseX = p.x;
        mouseY = p.y;
    }
    isClicking = TRUE;
    if (hClickerThread != nullptr) {
        // Stop and reset thread
        StopClicking();
    }
    hClickerThread = CreateThread(NULL, 0, ClickerFunc, currentClickerInfo, 0, &threadID);

}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GW2CLICKER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        switch(msg.message) {
            case WM_HOTKEY:
                POINT p;
                if (isClicking) {
                    StopClicking();
                }
                else {
                    StartClicking();
                }
                break;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

//
// Close the application
//
void CloseMe() {
    UnregisterHotKey(hWndMain, 2);
    PostQuitMessage(0);
}


// Clicker Function
//
DWORD WINAPI ClickerFunc(LPVOID lpParam) {
    P_CLICKER_INFO currentClickerInfo = (P_CLICKER_INFO)lpParam;
    int sleep_ms = currentClickerInfo->delay_in_ms;
    int rnd_range = sleep_ms / 100;
    int x_range = 15;
    int y_range = 15;

    // Prepare mouse input array
    INPUT inputs[4] = {};
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    inputs[0].mi.mouseData = XBUTTON1;
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    inputs[1].mi.mouseData = XBUTTON1;
    inputs[2].type = INPUT_MOUSE;
    inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    inputs[2].mi.mouseData = XBUTTON1;
    inputs[3].type = INPUT_MOUSE;
    inputs[3].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    inputs[3].mi.mouseData = XBUTTON1;

    UINT errors = 0;
    while (isClicking) {
        sleep_ms = currentClickerInfo->delay_in_ms + (((double)rand() / RAND_MAX) * (rnd_range * 2) - (rnd_range / 2));
        POINT p;
        GetCursorPos(&p);
        if (max(p.x, mouseX) - min(p.x, mouseX) > 20 ||
            max(p.y, mouseY) - min(p.y, mouseY) > 20) {
            StopClicking();
            return 0L;
        }
        p.x = mouseX;
        p.y = mouseY;
        inputs[0].mi.dx = currentClickerInfo->mouse_x + (((double)rand() / RAND_MAX) * (x_range * 2) - (x_range / 2));
        inputs[0].mi.dy = currentClickerInfo->mouse_y + (((double)rand() / RAND_MAX) * (y_range * 2) - (y_range / 2));
        inputs[1].mi.dx = inputs[0].mi.dx;
        inputs[1].mi.dy = inputs[0].mi.dy;
        inputs[2].mi.dx = inputs[0].mi.dx;
        inputs[2].mi.dy = inputs[0].mi.dy;
        inputs[3].mi.dx = inputs[0].mi.dx;
        inputs[3].mi.dy = inputs[0].mi.dy;

        UINT numCommands = (currentClickerInfo->doubleClick) ? 4 : 2;
        UINT uSent = SendInput(numCommands, inputs, sizeof(INPUT));
        if (uSent != numCommands) {
            // Did not send every command. Log it
            errors++;
            //Beep(500, 200);
        }
        else {
            //Beep(1000, 100);
        }
        Sleep(sleep_ms);
    }
    return 0L;
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = (HICON)LoadImage(wcex.hInstance, MAKEINTRESOURCE(IDI_APP), IMAGE_ICON, 16, 16, 0); // LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
   int wndW = 350, wndH = 125;
   std::wstringstream ss;
   ss << szTitle << " - Version ";
   ss << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_REVISION << "." << VERSION_BUILD;
   hWndMain = CreateWindowW(szWindowClass, ss.str().c_str(), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME ,
      CW_USEDEFAULT, 0, wndW, wndH, nullptr, nullptr, hInstance, nullptr);

   if (!hWndMain)
   {
      return FALSE;
   }
   // Draw controls
   //
   
   // Buttons
   //
   int btnW = (int)(wndW * 20) / 100;
   int btnH = (int)(btnW / 3);
   int btnMargin = int(btnW / 10);
   int offset = btnMargin;
   //HWND btnStart = CreateButton(hWnd, BTN_START, L"Start", offset, 100, btnW, btnH);
   offset += btnW + btnMargin;
   //HWND btnStop = CreateButton(hWnd, BTN_STOP, L"Stop", offset, 100, btnW, btnH);
   offset = (wndW / 2) - (btnW/2);
   HWND btnQuit = CreateButton(hWndMain, BTN_QUIT, L"Quit", offset, 50, btnW, btnH);

   // Mouse area
   // 
   //tbMouseX = CreateTextBox(hWnd, TB_MOUSEX, L"", 100, 20, 50, 20);
   //tbMouseY = CreateTextBox(hWnd, TB_MOUSEY, L"", 160, 20, 50, 20);


   // Profile combo box
   //
   int cbW = (wndW / 4) * 3;
   offset = (wndW / 2) - (cbW / 2);
   cbProfile = CreateComboBox(hWndMain, CB_PROFILE, offset, 15, cbW, 100);
   TCHAR A[32];
   memset(&A, 0, sizeof(A));
   for (int i = 0; i < ARRAYSIZE(Profiles); i++) {
        wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)Profiles[i]);
        SendMessage(cbProfile, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
   }
   SendMessage(cbProfile, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

   // Register HotKey
   //
   if (!RegisterHotKey(
       NULL,
       2,
       MOD_ALT | MOD_CONTROL | MOD_NOREPEAT,
       0x47 // g
   )) {
       MessageBox(NULL, L"Unable to register HotKey", szTitle, MB_ICONERROR);
   }

   ShowWindow(hWndMain, nCmdShow);
   UpdateWindow(hWndMain);
   //HICON icon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_APP), IMAGE_ICON, 16, 16, 0); // LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP), );
   //SendMessage(hWndMain, WM_SETICON, 1, (LPARAM)icon);

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
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            if (HIWORD(wParam) == BN_CLICKED) {
                std::wstringstream ss;
                ss << L"Button ID: ";
                ss << LOWORD(wParam);
                //MessageBox(NULL, ss.str().c_str(), L"Bøttån", MB_ICONERROR);
                switch (LOWORD(wParam)) {
                case BTN_QUIT:
                    CloseMe();
                }
            }
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
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
            RECT r;
            r.left = 0,
            r.right = 400;
            r.top = 0;
            r.bottom = 300;
            HBRUSH b = CreateSolidBrush(COLORREF(0x550055));
            FillRect(hdc, &r, b);            
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
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
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
