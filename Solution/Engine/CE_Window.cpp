#include "stdafx.h"

#include "CE_DirectX.h"
#include "CE_Window.h"

#include <dxgi.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include "CE_DirextXFactory.h"
#include "CE_WindowManager.h"
#include "CE_RendererProxy.h"

CE_Window::CE_Window()
{
}


CE_Window::~CE_Window()
{
	CE_SAFE_DELETE(myRendererProxy);
}

void CE_Window::Setup(const CE_Vector2i& aSize, const char* aTitle, CE_WindowManager* aWindowManager, WNDPROC aWinProc)
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

	CreateSwapchain();
	CreateBackbuffer();

	myRendererProxy = new CE_RendererProxy();
}

void CE_Window::PrepareForRender()
{
	float clearColor[4];
	clearColor[0] = 0.15f;
	clearColor[1] = 0.15f;
	clearColor[2] = 0.15f;
	clearColor[3] = 1.f;
	
	const CE_DirectX& directX = myWindowManager->GetDirectX();

	directX.GetContext()->ClearRenderTargetView(myBackbufferRenderTarget, clearColor);
	directX.GetContext()->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	directX.GetContext()->OMSetRenderTargets(1, &myBackbufferRenderTarget, myDepthStencilView);
	directX.GetContext()->RSSetViewports(1, myViewPort);
}

void CE_Window::FinishRender()
{
	mySwapChain->Present(0, 0);
	myRendererProxy->Clear();
}

void CE_Window::CreateSwapchain()
{
	const CE_DirectX& directX = myWindowManager->GetDirectX();

	IDXGIDevice* dxgiDevice = nullptr;
	HRESULT result = directX.GetDevice()->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	CE_ASSERT(SUCCEEDED(result), "Failed to get DXGIDevice");

	IDXGIAdapter* dxgiAdapter = nullptr;
	result = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	CE_ASSERT(SUCCEEDED(result), "Failed to get DXGIAdaptor");

	IDXGIFactory2* dxgiFactory = nullptr;
	result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgiFactory);
	CE_ASSERT(SUCCEEDED(result), "Failed to get DXGIFactory");

	DXGI_SWAP_CHAIN_DESC1 swapDesc;
	CE_ZERO_MEMORY(swapDesc);

	swapDesc.Width = myWindowSize.x;
	swapDesc.Height = myWindowSize.y;
	swapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.Stereo = false;
	swapDesc.SampleDesc.Count = 1;
	//swapDesc.SampleDesc.Quality = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapDesc.BufferCount = 1;
	swapDesc.Scaling = DXGI_SCALING_STRETCH;
	//swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//swapDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED


	result = dxgiFactory->CreateSwapChainForHwnd(
		directX.GetDevice(),
		myHWND,
		&swapDesc,
		NULL,
		NULL,
		&mySwapChain);
	CE_ASSERT(SUCCEEDED(result), "Failed to crete SwapChain");
}

void CE_Window::CreateBackbuffer()
{
	const CE_DirectX& directX = myWindowManager->GetDirectX();

	mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&myBackbuffer);
	directX.GetDevice()->CreateRenderTargetView(myBackbuffer, NULL, &myBackbufferRenderTarget);
	//aDirectX->GetDevice()->CreateShaderResourceView(myBackbuffer, NULL, &myBackbufferShaderResourceView);
	
	CE_DirextXFactory::GetInstance()->CreateTexture2D(myWindowSize, DXGI_FORMAT_D32_FLOAT, D3D11_BIND_DEPTH_STENCIL, 0, myDepthStencil);
	CE_DirextXFactory::GetInstance()->CreateDepthStencilView(DXGI_FORMAT_D32_FLOAT, D3D11_DSV_DIMENSION_TEXTURE2D, myDepthStencil, myDepthStencilView);
	
	CE_SAFE_DELETE(myViewPort);
	
	myViewPort = new D3D11_VIEWPORT();
	ZeroMemory(myViewPort, sizeof(D3D11_VIEWPORT));
	
	myViewPort->TopLeftX = 0;
	myViewPort->TopLeftY = 0;
	myViewPort->Width = static_cast<float>(myWindowSize.x);
	myViewPort->Height = static_cast<float>(myWindowSize.y);
	myViewPort->MinDepth = 0.f;
	myViewPort->MaxDepth = 1.f;
}