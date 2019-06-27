#include "stdafx.h"

#include "CE_DirectX.h"
#include "CE_DirextXFactory.h"
#include "CE_RendererProxy.h"
#include "CE_WindowManager.h"
#include "CE_Window.h"
#include "CE_SwapChain.h"
#include "CE_Camera.h"

#include "CUI_Manager.h"
#include "CE_Renderer.h"
#include "CE_DeferredRenderer.h"

CE_Window::CE_Window(const CE_Vector2i& aSize, const char* aTitle, CE_WindowManager* aWindowManager, WNDPROC aWinProc)
	: myClearColor(0.1f, 0.05f, 0.07f)
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

	CE_GPUContext& gpuContext = myWindowManager->GetGPUContext();
	mySwapChain = new CE_SwapChain(gpuContext, myWindowSize, myHWND);

	myCamera = new CE_Camera(myWindowSize);

	myDeferredRenderer = new CE_DeferredRenderer(gpuContext, *mySwapChain->GetBackbuffer(), myWindowSize);
	myRendererProxy = new CE_RendererProxy();

	myUIManager = new CUI_Manager();
}


CE_Window::~CE_Window()
{
	CE_SAFE_DELETE(myUIManager);
	CE_SAFE_DELETE(mySwapChain);
	CE_SAFE_DELETE(myCamera);
	CE_SAFE_DELETE(myRendererProxy);
	CE_SAFE_DELETE(myDeferredRenderer);
}

void CE_Window::BeginRender(CE_Renderer& aRenderer)
{
	myCamera->Update();

	myDeferredRenderer->ClearGBuffer(myClearColor);

	mySwapChain->PrepareForRender(myClearColor);
	aRenderer.UpdateConstantBuffers(*myCamera);
	myDeferredRenderer->UpdateConstantBuffers(*myCamera);
}

void CE_Window::Render(CE_Renderer& aRenderer)
{
	myDeferredRenderer->Render(aRenderer, GetRendererProxy());

	myUIManager->Render(*myRendererProxy);

	aRenderer.Render2D(*myRendererProxy);
}

void CE_Window::EndRender()
{
	mySwapChain->FinishRender();
	myRendererProxy->Clear();
}

void CE_Window::ProcessUI(const CE_Input& someInput)
{
	myUIManager->Update(someInput);
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
