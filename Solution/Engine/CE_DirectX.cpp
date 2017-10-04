#include "stdafx.h"
#include "CE_DirectX.h"
#include <dxgi.h>
#include <d3d11.h>
#include "CE_DirextXFactory.h"

CE_DirectX::CE_DirectX()
	: myDevice(nullptr)
	, myDeviceContext(nullptr)
	, myDebugInterface(nullptr)
{
	SetupDevice();

	CE_DirextXFactory::Create(myDevice, myDeviceContext);

	SetupDebugInterface();
}

CE_DirectX::~CE_DirectX()
{
	CE_DirextXFactory::Destroy();

	CE_SAFE_RELEASE(myDevice);

	myDeviceContext->ClearState();
	myDeviceContext->Flush();
	CE_SAFE_RELEASE(myDeviceContext);

#if _DEBUG
	myDebugInterface->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	CE_SAFE_RELEASE(myDebugInterface);
#endif
}

void CE_DirectX::SetupDevice()
{
	HRESULT result = D3D11CreateDevice(
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
		&myDevice,
		NULL,
		&myDeviceContext);

	CE_ASSERT(SUCCEEDED(result), "Failed to CreateDevice");
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