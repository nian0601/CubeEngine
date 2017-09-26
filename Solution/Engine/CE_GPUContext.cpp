#include "stdafx.h"

#include "CE_GPUContext.h"
#include "CE_DirectX.h"

CE_GPUContext::CE_GPUContext(const CE_WindowHandler* aWindowHandler)
{
	myDirectX = new CE_DirectX(aWindowHandler);
}

CE_GPUContext::~CE_GPUContext()
{
	CE_SAFE_DELETE(myDirectX);
}

void CE_GPUContext::EndFrame()
{
	myDirectX->EndFrame();
}

ID3D11Device* CE_GPUContext::GetDevice() const
{
	return myDirectX->GetDevice();
}

ID3D11DeviceContext* CE_GPUContext::GetContext() const
{
	return myDirectX->GetContext();
}

const CE_Vector2i& CE_GPUContext::GetWindowSize() const
{
	return myDirectX->GetWindowSize();
}
