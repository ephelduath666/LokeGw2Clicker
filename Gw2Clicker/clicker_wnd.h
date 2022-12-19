#pragma once

#include <sstream>
#include <windowsx.h>
#include "version.h"
#include "common.h"
#include "resource.h"

namespace loke {
namespace gw2clicker {

	static CLICKER_INFO _ciProfiles[3] = {
		{L"Candy Corn Gobler", 5200, 0, 0, TRUE},
		{L"Candy Cane Buff", 50, 0, 0, TRUE, 30},
		{L"Just get rid of the stack", 50, 0, 0, TRUE, 250}
	};

	static HWND _snumClickTb = nullptr;

	class ClickerWindow {
	private:
		HINSTANCE _hInstance	= nullptr;
		HWND _hWnd				= nullptr;
		HWND _tbNumClicks		= nullptr;
		HWND _cbProfile			= nullptr;
		int _windowH			= 150;
		int _windowW			= 350;
		BOOL _isActive = FALSE;

		WCHAR _szWindowClass[MAX_LOADSTRING];            // the main window class name


	public:
		ClickerWindow(HINSTANCE hInstance);
		~ClickerWindow();

		BOOL XWindow(int nCmdShow);
		int GetNumClicks();
		P_CLICKER_INFO GetProfile();
		static void SetActiveStatus(HWND hwnd, BOOL status);
		HWND GetHandler() { return _hWnd; }
		LPCWSTR GetHelpText() { return _szHelpText; }

	private:
		ClickerWindow();
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
		void LoadResources();
		static std::wstring GetTitle();
	};

}; };