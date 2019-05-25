#include "stdafx.h"

#include "CE_DirextXFactory.h"
#include "CE_GPUContext.h"
#include "CE_SwapChain.h"

#include <dxgi.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include "CE_Texture.h"


CE_SwapChain::CE_SwapChain(CE_GPUContext& aGPUContext, const CE_Vector2i& aWindowSize, HWND aHWND)
	: myGPUContext(aGPUContext)
{
	CreateSwapchain(aWindowSize, aHWND);
	CreateBackbuffer(aWindowSize);
}


CE_SwapChain::~CE_SwapChain()
{
	CE_SAFE_RELEASE(mySwapChain);
	CE_SAFE_DELETE(myViewPort);
}

void CE_SwapChain::PrepareForRender(const CE_Vector3f& aClearColor)
{
	float clearColor[4];
	clearColor[0] = aClearColor.x;
	clearColor[1] = aClearColor.y;
	clearColor[2] = aClearColor.z;
	clearColor[3] = 1.f;

	ID3D11DeviceContext* context = myGPUContext.GetContext();
	ID3D11RenderTargetView* target = myBackbuffer->GetRenderTarget();
	ID3D11DepthStencilView* stencil = myBackbuffer->GetDepthStencil();

	context->ClearRenderTargetView(target, clearColor);
	context->ClearDepthStencilView(stencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	context->RSSetViewports(1, myViewPort);
}

void CE_SwapChain::FinishRender()
{
	mySwapChain->Present(0, 0);
}

void CE_SwapChain::CreateSwapchain(const CE_Vector2i& aWindowSize, HWND aHWND)
{
	ID3D11Device* device = myGPUContext.GetDevice();

	IDXGIDevice* dxgiDevice = nullptr;
	CE_ASSERT(SUCCEEDED(device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice)), "Failed to get DXGIDevice");

	IDXGIAdapter* dxgiAdapter = nullptr;
	CE_ASSERT(SUCCEEDED(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter)), "Failed to get DXGIAdaptor");

	IDXGIFactory2* dxgiFactory = nullptr;
	CE_ASSERT(SUCCEEDED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgiFactory)), "Failed to get DXGIFactory");

	DXGI_SWAP_CHAIN_DESC1 swapDesc;
	CE_ZERO_MEMORY(swapDesc);

	swapDesc.Width = aWindowSize.x;
	swapDesc.Height = aWindowSize.y;
	swapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.Stereo = false;
	swapDesc.SampleDesc.Count = 1;
	//swapDesc.SampleDesc.Quality = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapDesc.BufferCount = 1;
	swapDesc.Scaling = DXGI_SCALING_STRETCH;
	//swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//swapDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED


	HRESULT result = dxgiFactory->CreateSwapChainForHwnd(
		device,
		aHWND,
		&swapDesc,
		NULL,
		NULL,
		&mySwapChain);
	CE_ASSERT(SUCCEEDED(result), "Failed to crete SwapChain");
}

void CE_SwapChain::CreateBackbuffer(const CE_Vector2i& aWindowSize)
{
	ID3D11Texture2D* texture = nullptr;
	mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&texture);

	myBackbuffer = new CE_Texture();
	myBackbuffer->InitAsBackBuffer(aWindowSize, texture, myGPUContext);

	CE_SAFE_DELETE(myViewPort);
	myViewPort = new D3D11_VIEWPORT();
	ZeroMemory(myViewPort, sizeof(D3D11_VIEWPORT));

	myViewPort->TopLeftX = 0;
	myViewPort->TopLeftY = 0;
	myViewPort->Width = static_cast<float>(aWindowSize.x);
	myViewPort->Height = static_cast<float>(aWindowSize.y);
	myViewPort->MinDepth = 0.f;
	myViewPort->MaxDepth = 1.f;
}

