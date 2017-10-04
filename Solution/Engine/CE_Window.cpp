#include "stdafx.h"
#include "CE_Window.h"


CE_Window::CE_Window()
{
}


CE_Window::~CE_Window()
{
}

void CE_Window::Setup(const CE_Vector2i& aSize, const char* aTitle, WNDPROC aWinProc)
{
	myWindowSize = aSize;

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = aWinProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hIcon = LoadIcon(wcex.hInstance, NULL);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = aTitle;
	wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);

	CE_ASSERT(RegisterClassEx(&wcex) != FALSE, "Failed to RegisterClassEx");

	RECT rc = { 0, 0, myWindowSize.x, myWindowSize.y };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	myHWND = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		aTitle,
		aTitle,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		this);

	CE_ASSERT(myHWND != nullptr, "Failed to CreateWindowEx");

	ShowWindow(myHWND, 10);
	UpdateWindow(myHWND);
}
