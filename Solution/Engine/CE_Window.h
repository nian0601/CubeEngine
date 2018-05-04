#pragma once

#include <windows.h>

class CE_Camera;
class CE_DirectX;
class CE_RendererProxy;
class CE_SwapChain;
class CE_Texture;
class CE_WindowManager;
class CE_Input;

struct CE_WindowMessage;

class CUI_Manager;

class CE_Window
{
public:
	CE_Window(const CE_Vector2i& aSize, const char* aTitle, CE_WindowManager* aWindowManager, WNDPROC aWinProc);
	~CE_Window();

	void PrepareForRender();
	void ProcessUI(const CE_Input& someInput);
	void FinishRender();

	bool HandleMessage(const CE_WindowMessage& aMessage);

	const CE_Vector2i& GetWindowSize() const { return myWindowSize; }
	const HWND& GetHWND() const { return myHWND; }

	CE_RendererProxy& GetRendererProxy() { return *myRendererProxy; }
	CE_Camera* GetCamera() { return myCamera; }

	CE_Texture* GetBackbuffer();

	CUI_Manager& GetUIManager() { return *myUIManager; }

private:
	CE_Vector2i myWindowSize;
	HWND myHWND;
	CE_WindowManager* myWindowManager;
	CE_RendererProxy* myRendererProxy;
	CE_Camera* myCamera;

	CE_SwapChain* mySwapChain;

	CUI_Manager* myUIManager;
};

