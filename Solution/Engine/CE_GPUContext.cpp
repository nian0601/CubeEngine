#include "stdafx.h"

#include "CE_GPUContext.h"
#include "CE_DirectX.h"

CE_GPUContext::CE_GPUContext(const CE_DirectX& aDirectX)
	: myDirectX(aDirectX)
{
}

CE_GPUContext::~CE_GPUContext()
{
}

ID3D11Device* CE_GPUContext::GetDevice() const
{
	return myDirectX.GetDevice();
}

ID3D11DeviceContext* CE_GPUContext::GetContext() const
{
	return myDirectX.GetContext();
}
