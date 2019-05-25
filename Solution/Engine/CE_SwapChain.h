#pragma once
#include <windows.h>

struct IDXGISwapChain1;
struct D3D11_VIEWPORT;

class CE_Texture;
class CE_GPUContext;
class CE_SwapChain
{
public:
	CE_SwapChain(CE_GPUContext& aGPUContext, const CE_Vector2i& aWindowSize, HWND aHWND);
	~CE_SwapChain();

	void PrepareForRender(const CE_Vector3f& aClearColor);
	void FinishRender();

	CE_Texture* GetBackbuffer() { return myBackbuffer; }

private:
	void CreateSwapchain(const CE_Vector2i& aWindowSize, HWND aHWND);
	void CreateBackbuffer(const CE_Vector2i& aWindowSize);

	IDXGISwapChain1* mySwapChain;
	CE_Texture* myBackbuffer;

	D3D11_VIEWPORT* myViewPort;

	CE_GPUContext& myGPUContext;
};

