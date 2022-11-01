#include "clicker_thread.h"

DWORD __stdcall loke::gw2clicker::ClickerThread::ClickerFunc(LPVOID lpParam)
{
    ThreadParam *tp = (ThreadParam*)lpParam;
    int sleep_ms = tp->currentClickerInfo->delay_in_ms;
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
    int loop = 0;
    SendMessage(tp->hMainWindow, WM_LOKE_CLICKER_STATUS, 0, TRUE);
    BOOL running = tp->parent->IsClicking();
    while (running) {
        sleep_ms = tp->currentClickerInfo->delay_in_ms + (((double)rand() / RAND_MAX) * (rnd_range * 2) - (rnd_range / 2));
        POINT p;
        GetCursorPos(&p);
        POINT mp = tp->parent->GetMousePos();
        if (max(p.x, mp.x) - min(p.x, mp.x) > 20 ||
            max(p.y, mp.y) - min(p.y, mp.y) > 20) {
            running = false;
        }
        else {
            p.x = mp.x;
            p.y = mp.y;
            inputs[0].mi.dx = tp->currentClickerInfo->mouse_x + (((double)rand() / RAND_MAX) * (x_range * 2) - (x_range / 2));
            inputs[0].mi.dy = tp->currentClickerInfo->mouse_y + (((double)rand() / RAND_MAX) * (y_range * 2) - (y_range / 2));
            inputs[1].mi.dx = inputs[0].mi.dx;
            inputs[1].mi.dy = inputs[0].mi.dy;
            inputs[2].mi.dx = inputs[0].mi.dx;
            inputs[2].mi.dy = inputs[0].mi.dy;
            inputs[3].mi.dx = inputs[0].mi.dx;
            inputs[3].mi.dy = inputs[0].mi.dy;

            UINT numCommands = (tp->currentClickerInfo->doubleClick) ? 4 : 2;
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
            loop++;
            if (tp->currentClickerInfo->num_clicks != 0) {
                if (loop >= tp->currentClickerInfo->num_clicks)
                    running = false;
            }
        }
    }
    SendMessage(tp->hMainWindow, WM_LOKE_CLICKER_STATUS, 0, FALSE);
    tp->parent->Stop();
    return 0L;
}

void loke::gw2clicker::ClickerThread::Start(HWND hMainWnd, loke::gw2clicker::P_CLICKER_INFO currentClickerInfo)
{
    POINT p;
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
        _mouseX = p.x;
        _mouseY = p.y;
    }
    _isClicking = TRUE;
    if (_hClickerThread != nullptr) {
        // Stop and reset thread
        Stop();
    }
    _tParam = new ThreadParam();
    _tParam->hMainWindow = hMainWnd;
    _tParam->parent = this;
    _tParam->currentClickerInfo = currentClickerInfo;
    _hClickerThread = CreateThread(NULL, 0, ClickerFunc, _tParam, 0, &_threadID);
}

void loke::gw2clicker::ClickerThread::Stop()
{
    _isClicking = FALSE;
    Beep(750, 80);
    Beep(750, 80);
    DWORD ret;
    //WaitForSingleObject(_hClickerThread, 5000);
    _hClickerThread = nullptr;
    _tParam = nullptr;
}

BOOL loke::gw2clicker::ClickerThread::IsClicking()
{
    return _isClicking;
}

POINT loke::gw2clicker::ClickerThread::GetMousePos()
{
    POINT p;
    p.x = _mouseX;
    p.y = _mouseY;
    return p;
}

loke::gw2clicker::ClickerThread::ClickerThread()
{
}

loke::gw2clicker::ClickerThread::~ClickerThread()
{
}
