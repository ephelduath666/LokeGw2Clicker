#pragma once
#include "framework.h"
#include <CommCtrl.h>

HWND CreateButton(HWND parent, int id, LPCWSTR text, int x, int y, int w, int h) {
	return CreateWindow(
		L"BUTTON",
		text,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		x,
		y,
		w,
		h,
		parent,
		(HMENU)id,
		(HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE),
		NULL
	);
}

HWND CreateTextBox(HWND parent, int id, LPCWSTR text, int x, int y, int w, int h) {
	return CreateWindow(
		L"EDIT",
		text,
		WS_BORDER | WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_LEFT,
		x,
		y,
		w,
		h,
		parent,
		(HMENU)id,
		(HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE),
		NULL
	);

}

HWND CreateComboBox(HWND parent, int id,int x, int y, int w, int h) {
	return CreateWindow(
		WC_COMBOBOX,
		TEXT("<Select profile>"),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_OVERLAPPED | WS_CHILD | WS_VISIBLE,
		x,
		y,
		w,
		h,
		parent,
		(HMENU)id,
		(HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE),
		NULL
	);

}

