#include "stdafx.h"

#include "CE_Window.h"
#include "CE_WindowManager.h"

#include <WinUser.h>

#define LOW_PART(l) ((unsigned short)(((unsigned long)(l)) & 0xffff))
#define HIGH_PART(l) ((unsigned short)((((unsigned long)(l)) >> 16) & 0xffff))

static MSG CE_WindowManager_EventMsg;
LRESULT CALLBACK CE_WindowManager_StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CE_WindowManager* windowManager = CE_WindowManager::GetInstance();

	const CE_GrowingArray<CE_Window*>& windows = windowManager->GetWindows();
	CE_Window* currentWindow = nullptr;
	for (CE_Window* window : windows)
	{
		if (window->GetHWND() == hWnd)
			currentWindow = window;
	}

	//CE_ASSERT(currentWindow != nullptr, "Got message that wasnt associated with any window??");

	if (currentWindow)
	{
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
		case WM_CHAR:			msg.myType = CE_WindowMessage::CHARACTER; break;
		}

		if (msg.myType != CE_WindowMessage::NONE)
		{
			msg.myLParams = lParam;
			msg.myWParams = wParam;

			msg.myLParts.myLow = LOW_PART(msg.myLParams);
			msg.myLParts.myHigh = HIGH_PART(msg.myLParams);

			msg.myWParts.myLow = LOW_PART(msg.myWParams);
			msg.myWParts.myHigh = HIGH_PART(msg.myWParams);

			windowManager->HandleWindowMessage(currentWindow, msg);
			return 0;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

CE_WindowManager* CE_WindowManager::ourInstance = nullptr;
void CE_WindowManager::Create(CE_GPUContext& aGPUContext)
{
	ourInstance = new CE_WindowManager(aGPUContext);
}

void CE_WindowManager::Destory()
{
	CE_SAFE_DELETE(ourInstance);
}

CE_WindowManager* CE_WindowManager::GetInstance()
{
	return ourInstance;
}

CE_Window* CE_WindowManager::CreateNewWindow(const CE_Vector2i& aSize, const char* aTitle)
{
	CE_Window* window = new CE_Window(aSize, aTitle, this, CE_WindowManager_StaticWndProc);
	myWindows.Add(window);

	return window;
}

bool CE_WindowManager::PumpEvent()
{
	if (PeekMessage(&CE_WindowManager_EventMsg, NULL, 0, 0, PM_REMOVE))
	{
		UINT msg = CE_WindowManager_EventMsg.message;
		if (msg == WM_QUIT)
			return false;

		TranslateMessage(&CE_WindowManager_EventMsg);
		DispatchMessage(&CE_WindowManager_EventMsg);
	}

	return true;
}

void CE_WindowManager::HandleWindowMessage(CE_Window* aWindow, const CE_WindowMessage& aMessage)
{
	switch (aMessage.myType)
	{
	case CE_WindowMessage::DESTROY:
		PostQuitMessage(0);
		return;
	case CE_WindowMessage::CHARACTER:
		if (!aWindow->HandleMessage(aMessage))
		{
			//Feed it into inputmanager
		}
		return;
	}

	aWindow->HandleMessage(aMessage);
}

CE_WindowManager::CE_WindowManager(CE_GPUContext& aGPUContext)
	: myGPUContext(aGPUContext)
{
}


CE_WindowManager::~CE_WindowManager()
{
	myWindows.DeleteAll();
}