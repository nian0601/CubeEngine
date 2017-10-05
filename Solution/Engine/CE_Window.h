#pragma once

#include <windows.h>

struct IDXGISwapChain;
struct IDXGISwapChain1;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;

struct D3D11_VIEWPORT;

class CE_DirectX;
class CE_RendererProxy;
class CE_WindowManager;

class CE_Window
{
public:
	CE_Window();
	~CE_Window();

	void Setup(const CE_Vector2i& aSize, const char* aTitle, CE_WindowManager* aWindowManager, WNDPROC aWinProc);

	const CE_Vector2i& GetWindowSize() const { return myWindowSize; }
	const HWND& GetHWND() const { return myHWND; }

	void PrepareForRender();

	void FinishRender();

	CE_RendererProxy& GetRendererProxy() { return *myRendererProxy; }

private:
	void CreateSwapchain();
	void CreateBackbuffer();

	CE_Vector2i myWindowSize;
	HWND myHWND;
	CE_WindowManager* myWindowManager;
	CE_RendererProxy* myRendererProxy;

	IDXGISwapChain1* mySwapChain;

	ID3D11Texture2D* myBackbuffer;
	ID3D11RenderTargetView* myBackbufferRenderTarget;

	ID3D11Texture2D* myDepthStencil;
	ID3D11DepthStencilView* myDepthStencilView;
	
	D3D11_VIEWPORT* myViewPort;
};

