#pragma once

#include <windows.h>

struct IDXGISwapChain;
struct ID3D11RenderTargetView;

class CE_Window
{
public:
	CE_Window();
	~CE_Window();

	void Setup(const CE_Vector2i& aSize, const char* aTitle, WNDPROC aWinProc);

	const CE_Vector2i& GetWindowSize() const { return myWindowSize; }
	const HWND& GetHWND() const { return myHWND; }

private:

	CE_Vector2i myWindowSize;
	HWND myHWND;

	IDXGISwapChain* mySwapChain;
	ID3D11RenderTargetView* myBackbufferRenderTarget;
};

