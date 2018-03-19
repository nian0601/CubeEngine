#include "stdafx.h"

#include "CE_DirectX.h"
#include "CE_DirextXFactory.h"
#include "CE_RendererProxy.h"
#include "CE_WindowManager.h"
#include "CE_Window.h"
#include "CE_SwapChain.h"

CE_Window::CE_Window(const CE_Vector2i& aSize, const char* aTitle, CE_WindowManager* aWindowManager, WNDPROC aWinProc)
{
	myWindowManager = aWindowManager;
	myWindowSize = aSize;
	myOrthagonalProjection = CE_Matrix44f::CreateOrthogonalMatrixLH(static_cast<float>(myWindowSize.x), static_cast<float>(myWindowSize.y), 0.1f, 100.f);

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
		myWindowManager);

	CE_ASSERT(myHWND != nullptr, "Failed to CreateWindowEx");

	ShowWindow(myHWND, 10);
	UpdateWindow(myHWND);

	mySwapChain = new CE_SwapChain(myWindowManager->GetGPUContext(), aSize, myHWND);
	myRendererProxy = new CE_RendererProxy();
}


CE_Window::~CE_Window()
{
	CE_SAFE_DELETE(myRendererProxy);
	CE_SAFE_DELETE(mySwapChain);
}

void CE_Window::PrepareForRender()
{
	mySwapChain->PrepareForRender();
}

void CE_Window::FinishRender()
{
	mySwapChain->FinishRender();
	myRendererProxy->Clear();
}

CE_Texture* CE_Window::GetBackbuffer()
{
	return mySwapChain->GetBackbuffer();
}
