// Gw2Clicker.cpp : Defines the entry point for the application.
//

#include "Gw2Clicker.h"
#include "clicker_wnd.h"
#include "clicker_thread.h"

// Global Variables:
loke::gw2clicker::ClickerWindow *_ptrWindow = nullptr;
loke::gw2clicker::ClickerThread* _ptrClicker = nullptr;

HINSTANCE hInst;                                // current instance


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    _ptrWindow = new loke::gw2clicker::ClickerWindow(hInstance);
    _ptrClicker = new loke::gw2clicker::ClickerThread();

    // Perform application initialization:
    if (!_ptrWindow->XWindow(nCmdShow))
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
                    if (_ptrClicker->IsClicking()) {
                        _ptrClicker->Stop();
                    }
                    else {
                        _ptrClicker->Start(_ptrWindow->GetHandler(), _ptrWindow->GetProfile());
                    }
                    break;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}
