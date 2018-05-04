#include "stdafx.h"

#include "CE_DirectX.h"
#include "CE_DirextXFactory.h"
#include "CE_RendererProxy.h"
#include "CE_WindowManager.h"
#include "CE_Window.h"
#include "CE_SwapChain.h"
#include "CE_Camera.h"

#include "CUI_Manager.h"

CE_Window::CE_Window(const CE_Vector2i& aSize, const char* aTitle, CE_WindowManager* aWindowManager, WNDPROC aWinProc)
{
	myWindowManager = aWindowManager;
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
		myWindowManager);

	CE_ASSERT(myHWND != nullptr, "Failed to CreateWindowEx");

	ShowWindow(myHWND, 10);
	UpdateWindow(myHWND);

	myRendererProxy = new CE_RendererProxy();
	myCamera = new CE_Camera(myWindowSize);
	mySwapChain = new CE_SwapChain(myWindowManager->GetGPUContext(), myWindowSize, myHWND);

	myUIManager = new CUI_Manager();
}


CE_Window::~CE_Window()
{
	CE_SAFE_DELETE(myUIManager);
	CE_SAFE_DELETE(mySwapChain);
	CE_SAFE_DELETE(myCamera);
	CE_SAFE_DELETE(myRendererProxy);
}

void CE_Window::PrepareForRender()
{
	mySwapChain->PrepareForRender();
}

void CE_Window::ProcessUI(const CE_Input& someInput)
{
	myUIManager->Update(someInput);
	myUIManager->Render(*myRendererProxy);
}

void CE_Window::FinishRender()
{
	mySwapChain->FinishRender();
	myRendererProxy->Clear();
}

bool CE_Window::HandleMessage(const CE_WindowMessage& aMessage)
{
	switch (aMessage.myType)
	{
		case CE_WindowMessage::PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(myHWND, &ps);
			EndPaint(myHWND, &ps);
			return true;
		}
		case CE_WindowMessage::SIZE:
		{
			int newX = aMessage.myLParts.myLow;
			int newY = aMessage.myLParts.myHigh;
			newX;
			newY;
			return true;
		}
	}
	return myUIManager->OnWindowMessage(aMessage);
}

CE_Texture* CE_Window::GetBackbuffer()
{
	return mySwapChain->GetBackbuffer();
}
