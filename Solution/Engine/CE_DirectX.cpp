#include "stdafx.h"
#include "CE_DirectX.h"
#include <dxgi.h>
#include "CE_WindowHandler.h"
#include <d3d11.h>
#include "CE_DirextXFactory.h"

CE_DirectX::CE_DirectX(const CE_WindowHandler* aWindowHandler)
	: myWindowHandler(aWindowHandler)
	, myDevice(nullptr)
	, myDeviceContext(nullptr)
	, mySwapChain(nullptr)
	, myDebugInterface(nullptr)
	, myViewPort(nullptr)
	, myBackbuffer(nullptr)
	, myBackbufferRenderTarget(nullptr)
	, myBackbufferShaderResourceView(nullptr)
	, myDepthStencil(nullptr)
	, myDepthStencilView(nullptr)
{
	SetupSwapChain();

	CE_DirextXFactory::Create(myDevice, myDeviceContext);

	SetupDebugInterface();
	SetupBackbuffer();
}

CE_DirectX::~CE_DirectX()
{
	CE_DirextXFactory::Destroy();

	CE_SAFE_RELEASE(mySwapChain);

	CE_SAFE_RELEASE(myBackbuffer);
	CE_SAFE_RELEASE(myBackbufferRenderTarget);
	CE_SAFE_RELEASE(myBackbufferShaderResourceView);

	CE_SAFE_RELEASE(myDepthStencil);
	CE_SAFE_RELEASE(myDepthStencilView);

	CE_SAFE_RELEASE(myDevice);

	CE_SAFE_DELETE(myViewPort);

	myDeviceContext->ClearState();
	myDeviceContext->Flush();
	CE_SAFE_RELEASE(myDeviceContext);

#if _DEBUG
	myDebugInterface->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	CE_SAFE_RELEASE(myDebugInterface);
#endif
}

void CE_DirectX::EndFrame()
{
	mySwapChain->Present(0, 0);
	float clearColor[4];
	clearColor[0] = 0.15f;
	clearColor[1] = 0.15f;
	clearColor[2] = 0.15f;
	clearColor[3] = 1.f;

	myDeviceContext->ClearRenderTargetView(myBackbufferRenderTarget, clearColor);
	myDeviceContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

const CE_Vector2i& CE_DirectX::GetWindowSize() const
{
	return myWindowHandler->GetWindowSize();
}

void CE_DirectX::SetupSwapChain()
{
	int numerator = 0;
	int denmoinator = 0;
	GetRefreshRate(numerator, denmoinator);

	DXGI_SWAP_CHAIN_DESC swapDesc;
	CE_ZERO_MEMORY(swapDesc);

	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = myWindowHandler->GetWindowSize().x;
	swapDesc.BufferDesc.Height = myWindowHandler->GetWindowSize().y;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = numerator;
	swapDesc.BufferDesc.RefreshRate.Denominator = denmoinator;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapDesc.OutputWindow = myWindowHandler->GetHWND();
	swapDesc.SampleDesc.Count = 1;
	swapDesc.Windowed = true;

	HRESULT result = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
#ifdef _DEBUG
		D3D11_CREATE_DEVICE_DEBUG,
#else
		NULL,
#endif
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapDesc,
		&mySwapChain,
		&myDevice,
		nullptr,
		&myDeviceContext);

	CE_ASSERT(SUCCEEDED(result) == TRUE, "Failed to CreateDeviceAndSwapChain");
}

void CE_DirectX::SetupDebugInterface()
{
#ifdef _DEBUG
	myDebugInterface = nullptr;
	HRESULT result = myDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&myDebugInterface);
	CE_ASSERT(SUCCEEDED(result) == TRUE, "Failed to Query DebugInterface");


	ID3D11InfoQueue* infoQueue = nullptr;
	CE_ASSERT(SUCCEEDED(myDebugInterface->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&infoQueue)) == TRUE, "Failed to Query InfoQueue");
	infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
	infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);

	D3D11_MESSAGE_ID hide[] =
	{
		D3D11_MESSAGE_ID_DEVICE_PSSETSHADERRESOURCES_HAZARD,
		D3D11_MESSAGE_ID_DEVICE_OMSETRENDERTARGETS_HAZARD
		// Add more message IDs here as needed
	};

	D3D11_INFO_QUEUE_FILTER filter;
	memset(&filter, 0, sizeof(filter));
	filter.DenyList.NumIDs = _countof(hide);
	filter.DenyList.pIDList = hide;
	infoQueue->AddStorageFilterEntries(&filter);
	infoQueue->Release();
#endif
}

void CE_DirectX::SetupBackbuffer()
{
	mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&myBackbuffer);
	myDevice->CreateRenderTargetView(myBackbuffer, NULL, &myBackbufferRenderTarget);
	myDevice->CreateShaderResourceView(myBackbuffer, NULL, &myBackbufferShaderResourceView);

	CE_DirextXFactory::GetInstance()->CreateTexture2D(myWindowHandler->GetWindowSize(), DXGI_FORMAT_D32_FLOAT, D3D11_BIND_DEPTH_STENCIL, 0, myDepthStencil);
	CE_DirextXFactory::GetInstance()->CreateDepthStencilView(DXGI_FORMAT_D32_FLOAT, D3D11_DSV_DIMENSION_TEXTURE2D, myDepthStencil, myDepthStencilView);
	
	myDeviceContext->OMSetRenderTargets(1, &myBackbufferRenderTarget, myDepthStencilView);

	CE_SAFE_DELETE(myViewPort);

	myViewPort = new D3D11_VIEWPORT();
	ZeroMemory(myViewPort, sizeof(D3D11_VIEWPORT));

	myViewPort->TopLeftX = 0;
	myViewPort->TopLeftY = 0;
	myViewPort->Width = static_cast<float>(myWindowHandler->GetWindowSize().x);
	myViewPort->Height = static_cast<float>(myWindowHandler->GetWindowSize().y);
	myViewPort->MinDepth = 0.f;
	myViewPort->MaxDepth = 1.f;

	myDeviceContext->RSSetViewports(1, myViewPort);
}

void CE_DirectX::GetRefreshRate(int& aNumeratorOut, int& aDenominatorOut) const
{
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes;
	DXGI_MODE_DESC* displayModeList;

	HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	result = factory->EnumAdapters(0, &adapter);
	result = adapter->EnumOutputs(0, &adapterOutput);
	adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	displayModeList = new DXGI_MODE_DESC[numModes];
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);

	const CE_Vector2i& screenSize = myWindowHandler->GetWindowSize();
	for (unsigned int i = 0; i < numModes; ++i)
	{
		int width = static_cast<int>(displayModeList[i].Width);
		int height = static_cast<int>(displayModeList[i].Height);
		if (width == screenSize.x && height == screenSize.y)
		{
			aNumeratorOut = displayModeList[i].RefreshRate.Numerator;
			aDenominatorOut = displayModeList[i].RefreshRate.Denominator;
		}
	}

	CE_SAFE_DELETE_ARRAY(displayModeList);
	CE_SAFE_RELEASE(adapterOutput);
	CE_SAFE_RELEASE(adapter);
	CE_SAFE_RELEASE(factory);
}
