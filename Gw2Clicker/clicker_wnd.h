#pragma once

#include <sstream>
#include <windowsx.h>
#include "version.h"
#include "common.h"
#include "resource.h"

#define MAX_LOADSTRING 100

namespace loke {
namespace gw2clicker {
	class ClickerWindow {
	private:
		HINSTANCE _hInstance	= nullptr;
		HWND _hWnd				= nullptr;
		HWND _tbNumClicks		= nullptr;
		HWND _cbProfile			= nullptr;
		int _windowH			= 150;
		int _windowW			= 350;

		CLICKER_INFO _ciProfiles[3] = {
			{L"Candy Corn Gobler", 5200, 0, 0, TRUE},
			{L"Snow Globe Gobler", 100, 0, 0, TRUE},
			{L"Just get rid of the stack", 50, 0, 0, TRUE}
		};

		WCHAR _szTitle[MAX_LOADSTRING];                  // The title bar text
		WCHAR _szWindowClass[MAX_LOADSTRING];            // the main window class name


	public:
		ClickerWindow(HINSTANCE hInstance);
		~ClickerWindow();

		BOOL XWindow(int nCmdShow);
		int GetNumClicks();
		P_CLICKER_INFO GetProfile();
	private:
		ClickerWindow();
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void LoadResources();
	};

}; };