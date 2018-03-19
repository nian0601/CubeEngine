#pragma once

#include <windows.h>

class CE_DirectX;
class CE_RendererProxy;
class CE_SwapChain;
class CE_Texture;
class CE_WindowManager;

class CE_Window
{
public:
	CE_Window(const CE_Vector2i& aSize, const char* aTitle, CE_WindowManager* aWindowManager, WNDPROC aWinProc);
	~CE_Window();

	const CE_Vector2i& GetWindowSize() const { return myWindowSize; }
	const HWND& GetHWND() const { return myHWND; }

	void PrepareForRender();
	void FinishRender();

	CE_RendererProxy& GetRendererProxy() { return *myRendererProxy; }
	const CE_Matrix44f& GetOrthagonalProjection() const { return myOrthagonalProjection; }

	CE_Texture* GetBackbuffer();

private:
	CE_Matrix44f myOrthagonalProjection;
	CE_Vector2i myWindowSize;
	HWND myHWND;
	CE_WindowManager* myWindowManager;
	CE_RendererProxy* myRendererProxy;

	CE_SwapChain* mySwapChain;
};

