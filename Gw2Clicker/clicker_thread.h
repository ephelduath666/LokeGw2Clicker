#pragma once
#include "common.h"

namespace loke {
	namespace gw2clicker {
		class ThreadParam;

		class ClickerThread {
		private:
			BOOL _isClicking = FALSE;
			static DWORD WINAPI ClickerFunc(LPVOID lpParam);
			int _mouseX = 0, _mouseY = 0;
			HANDLE _hClickerThread;
			DWORD _threadID = 0;
			ThreadParam *_tParam = nullptr;

		public:
			void Start(loke::gw2clicker::P_CLICKER_INFO currentClickerInfo);
			void Stop();
			BOOL IsClicking();
			POINT GetMousePos();
			ClickerThread();
			~ClickerThread();
		};

		class ThreadParam {
		public:
			ClickerThread* parent = nullptr;
			loke::gw2clicker::P_CLICKER_INFO currentClickerInfo = nullptr;
		};

	};
};
