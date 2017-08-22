#include "stdafx.h"
#include "CE_WindowHandler.h"

#include <windows.h>
#include <WinUser.h>

static HWND CE_WindowHandler_Hwnd;
static MSG CE_WindowHandler_EventMsg;
LRESULT CALLBACK CE_WindowHandler_StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CE_WindowHandler* windowHandler = nullptr;
	if (message == WM_NCCREATE)
	{
		windowHandler = static_cast<CE_WindowHandler*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetLastError(0);

		if (!SetWindowLongPtr(hWnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(windowHandler)))
		{
			if (GetLastError() != 0)
			{
				//DL_ASSERT("Failed to setup WndProc");
				return FALSE;
			}
		}
	}
	else
	{
		windowHandler = reinterpret_cast<CE_WindowHandler*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	CE_WindowMessage msg;
	msg.myType = CE_WindowMessage::NONE;

	switch (message)
	{
	case WM_PAINT:			msg.myType = CE_WindowMessage::PAINT; break;
	case WM_DESTROY:		msg.myType = CE_WindowMessage::DESTROY; break;
	case WM_ACTIVATE:		msg.myType = CE_WindowMessage::ACTIVATE; break;
	case WM_SIZE:			msg.myType = CE_WindowMessage::SIZE; break;
	case WM_ENTERSIZEMOVE:	msg.myType = CE_WindowMessage::ENTER_SIZE_MOVE; break;
	case WM_EXITSIZEMOVE:	msg.myType = CE_WindowMessage::EXIT_SIZE_MOVE; break;
	}

	if (msg.myType != CE_WindowMessage::NONE)
	{
		msg.myLowWordLParam = LOWORD(lParam);
		msg.myHighWordLParam = HIWORD(lParam);
		msg.myLowWordWParam = LOWORD(wParam);
		msg.myHighWordWParam = HIWORD(wParam);

		windowHandler->HandleWindowMessage(msg);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

CE_WindowHandler::CE_WindowHandler(int aWidth, int aHeight)
{
	const char* windowTitle = "Cube Engine";

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = CE_WindowHandler_StaticWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hIcon = LoadIcon(wcex.hInstance, NULL);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = windowTitle;
	wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);

	RegisterClassEx(&wcex);

	//DL_ASSERT_EXP(RegisterClassEx(&wcex) != FALSE, "Failed to RegisterClassEx");

	RECT rc = { 0, 0, aWidth, aHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	CE_WindowHandler_Hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		windowTitle,
		windowTitle,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		this);

	//DL_ASSERT_EXP(myHwnd != nullptr, "Failed to CreateWindowEx");

	ShowWindow(CE_WindowHandler_Hwnd, 10);
	UpdateWindow(CE_WindowHandler_Hwnd);
}

bool CE_WindowHandler::PumpEvent()
{
	if (PeekMessage(&CE_WindowHandler_EventMsg, NULL, 0, 0, PM_REMOVE))
	{
		UINT msg = CE_WindowHandler_EventMsg.message;
		if (msg == WM_QUIT)
			return false;

		TranslateMessage(&CE_WindowHandler_EventMsg);
		DispatchMessage(&CE_WindowHandler_EventMsg);
	}

	return true;
}

void CE_WindowHandler::HandleWindowMessage(const CE_WindowMessage& aMessage)
{
	switch (aMessage.myType)
	{
	case CE_WindowMessage::PAINT:
		PAINTSTRUCT ps;
		BeginPaint(CE_WindowHandler_Hwnd, &ps);
		EndPaint(CE_WindowHandler_Hwnd, &ps);
		break;
	case CE_WindowMessage::DESTROY:
		PostQuitMessage(0);
		break;
	case CE_WindowMessage::ACTIVATE:
		break;
	case CE_WindowMessage::SIZE:
		//int newX = aMessage.myLowWordLParam;
		//int newY = aMessage.myHighWordLParam;
		break;
	case CE_WindowMessage::ENTER_SIZE_MOVE:
		break;
	case CE_WindowMessage::EXIT_SIZE_MOVE:
		break;
	}
}

