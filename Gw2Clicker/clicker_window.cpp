#include "clicker_wnd.h"
#include "Controls.h"
#include <vector>
#include <string>

loke::gw2clicker::ClickerWindow::~ClickerWindow() {
    DeleteObject(_brshSolidWindowColor);
    UnregisterHotKey(_hWnd, 2);
}

loke::gw2clicker::ClickerWindow::ClickerWindow() {
    LoadResources();
}

loke::gw2clicker::ClickerWindow::ClickerWindow(HINSTANCE hInstance) : ClickerWindow() {
    WNDCLASSEXW wcex;
    _hInstance = hInstance;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = &loke::gw2clicker::ClickerWindow::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = _szWindowClass;
    wcex.hIconSm = (HICON)LoadImage(wcex.hInstance, MAKEINTRESOURCE(IDI_APP), IMAGE_ICON, 16, 16, 0); // LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);
}

BOOL loke::gw2clicker::ClickerWindow::XWindow(int nCmdShow) {
    _hWnd = CreateWindowW(_szWindowClass, GetTitle().c_str(), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME,
        CW_USEDEFAULT, 0, _windowW, _windowH, nullptr, nullptr, _hInstance, nullptr);

    if (!_hWnd)
    {
        return FALSE;
    }
    // Draw controls
    //

    // Buttons
    //
    int btnW = (int)(_windowW * 20) / 100;
    int btnH = (int)(btnW / 3);
    int offset = (_windowW / 2) - (btnW / 2);
    int offsetY = _windowH - btnH * 3;
    HWND btnQuit = loke::CreateButton(_hWnd, BTN_QUIT, L"&Quit", offset, offsetY, btnW, btnH);
    HWND btnAbout = loke::CreateButton(_hWnd, BTN_ABOUT, L"&About", _windowW - btnW - 20, offsetY, btnW, btnH);
    // Mouse area
    // 
    //tbMouseX = CreateTextBox(hWnd, TB_MOUSEX, L"", 100, 20, 50, 20);
    //tbMouseY = CreateTextBox(hWnd, TB_MOUSEY, L"", 160, 20, 50, 20);


    // Profile combo box
    //
    int cbW = (_windowW / 4) * 3;
    offset = (_windowW / 2) - (cbW / 2);
    _cbProfile = loke::CreateComboBox(_hWnd, CB_PROFILE, offset, 15, cbW, 100);

    TCHAR A[32];
    memset(&A, 0, sizeof(A));
    for (int i = 0; i < ARRAYSIZE(_ciProfiles); i++) {
        wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)_ciProfiles[i].name);
        SendMessage(_cbProfile, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
    }
    SendMessage(_cbProfile, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

    // Config
    //
    SIZE lpsText;
    HWND lbl1 = loke::CreateLabel(_hWnd, L"Num clicks:", offset, 50, 100, 15);
    HDC hdc = GetDC(lbl1);
    GetTextExtentPoint32(hdc, L"Num clicks:", 11, &lpsText);
    int tbW = lpsText.cx;
    GetTextExtentPoint32(hdc, L"88888", 5, &lpsText);
    //HWND lbl = CreateTextBox(hWndMain, 4, L"100", offset, 45, 50, 10);
    _tbNumClicks = loke::CreateNumericBox(_hWnd, TB_NUM_CLICKS, L"", offset + tbW + 10, 48, lpsText.cx, lpsText.cy + 4);
    SendMessage(_tbNumClicks, (UINT)EM_SETLIMITTEXT, (WPARAM)4, (LPARAM)0);
    _snumClickTb = _tbNumClicks;

    // Register HotKey
    //
    if (!RegisterHotKey(
        NULL,
        2,
        MOD_ALT | MOD_CONTROL | MOD_NOREPEAT,
        0x47 // g
    )) {
        MessageBox(NULL, L"Unable to register HotKey", _szTitle, MB_ICONERROR);
    }

    // Load brushes
    _brshSolidWindowColor = CreateSolidBrush(_bgColor);

    ShowWindow(_hWnd, nCmdShow);
    UpdateWindow(_hWnd);
    //HICON icon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_APP), IMAGE_ICON, 16, 16, 0); // LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP), );
    //SendMessage(hWndMain, WM_SETICON, 1, (LPARAM)icon);

    return TRUE;
}

LRESULT CALLBACK loke::gw2clicker::ClickerWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg)
    {
    case WM_COMMAND:
    {
        if (HIWORD(wParam) == BN_CLICKED) {
            switch (LOWORD(wParam)) {
            case BTN_ABOUT:
                DialogBoxParamW(NULL, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, (DLGPROC)&ClickerWindow::About, 0L);
                break;
            case BTN_QUIT:
                PostQuitMessage(0);
            }
        }
        else if (HIWORD(wParam) == CBN_SELENDOK) {
            switch (LOWORD(wParam)) {
            case CB_PROFILE:
                // User selecte a profile
                int ix = SendMessage((HWND)lParam, CB_GETCURSEL, 0L, 0L);
                wchar_t buff[4];
                if (_ciProfiles[ix].num_clicks == 0)
                {
                    wcscpy_s(buff, 4, L"");
                }
                else {                   
                    _itow_s(_ciProfiles[ix].num_clicks, buff, 10);
                }
                SendMessage(_snumClickTb, WM_SETTEXT, FALSE, (LPARAM)buff);
            }
        }
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }
    break;
    case WM_LOKE_CLICKER_STATUS:
    {
        SetActiveStatus(hwnd, (BOOL)lParam);
    }
    break;
    case WM_CTLCOLORSTATIC:
    {
        SetTextColor((HDC)wParam, RGB(255, 255, 255));
        SetBkColor((HDC)wParam, _bgColor);
        return (INT_PTR)_brshSolidWindowColor;
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        RECT r;
        r.left = 0,
            r.right = 400;
        r.top = 0;
        r.bottom = 300;
        FillRect(hdc, &r, _brshSolidWindowColor);
        EndPaint(hwnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

INT_PTR loke::gw2clicker::ClickerWindow::About(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
    {
        SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, FALSE, (LPARAM)_szHelpText);
        /*
        wchar_t* tok = NULL;
        wchar_t* next_tok = NULL;
        tok = wcstok_s(_szHelpText, L"\n", &next_tok);
        while (tok != NULL) {
            int textLen = SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_GETTEXTLENGTH, (WPARAM)0, (LPARAM)0);
            SendMessage(GetDlgItem(hwnd, IDC_EDIT1), EM_SETSEL, (WPARAM)0, (LPARAM)textLen);
            SendMessage(GetDlgItem(hwnd, IDC_EDIT1), EM_REPLACESEL, FALSE, (LPARAM)tok);
            tok = wcstok_s(NULL, L"\n", &next_tok);
        }
        */
    }
    return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hwnd, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


void loke::gw2clicker::ClickerWindow::LoadResources() {
    LoadStringW(_hInstance, IDS_APP_TITLE, _szTitle, MAX_LOADSTRING);
    LoadStringW(_hInstance, IDS_USAGE, _szHelpText, 1024);
    LoadStringW(_hInstance, IDC_GW2CLICKER, _szWindowClass, MAX_LOADSTRING);

}

std::wstring loke::gw2clicker::ClickerWindow::GetTitle()
{
    std::wstringstream ss;
    ss << _szTitle << " - Version ";
    ss << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_REVISION << "." << VERSION_BUILD;
    return ss.str();
}

int loke::gw2clicker::ClickerWindow::GetNumClicks() {
    int numClicks = 0;
    wchar_t tcNumClicks[4] = { 0 };
    SendMessage(_tbNumClicks, WM_GETTEXT, (WPARAM)4, (LPARAM)tcNumClicks);
    if (wcslen(tcNumClicks) > 0) {
        numClicks = _wtoi(tcNumClicks);
    }
    else {
        numClicks = 0;
    }
    return numClicks;
}


loke::gw2clicker::P_CLICKER_INFO loke::gw2clicker::ClickerWindow::GetProfile() {
    int ix = ComboBox_GetCurSel(_cbProfile);
    if(_ciProfiles[ix].num_clicks <= 0)
        _ciProfiles[ix].num_clicks = GetNumClicks();
    return &_ciProfiles[ix];
}

void loke::gw2clicker::ClickerWindow::SetActiveStatus(HWND hwnd, BOOL status)
{
    if (status) {
        std::wstringstream ss;
        ss << GetTitle() << " - ACTIVE";
        SetWindowText(hwnd, ss.str().c_str());
    }
    else {
        SetWindowText(hwnd, GetTitle().c_str());
    }
}
