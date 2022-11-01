#pragma once
#include "framework.h"

namespace loke {
	namespace gw2clicker {
		static HBRUSH _brshSolidWindowColor = nullptr;
		static COLORREF _bgColor = COLORREF(0x550055);

		typedef struct ClickerInfo {
			LPCWSTR name = L"";
			int delay_in_ms = 1000;
			int mouse_x = 0;
			int mouse_y = 0;
			bool doubleClick = FALSE;
			int num_clicks = 0;
		} CLICKER_INFO, * P_CLICKER_INFO;

	};
};