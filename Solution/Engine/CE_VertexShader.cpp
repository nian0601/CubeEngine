#include "stdafx.h"

#include "CE_Camera.h"
#include "CE_GPUContext.h"
#include "CE_VertexShader.h"

#include <d3d11.h>
#include <d3dcompiler.h>

#include "CE_GPUContext.h"

CE_VertexShader::CE_VertexShader(const char* aFilePath, CE_GPUContext& aGPUContext)
	: CE_GenericShader(aFilePath, aGPUContext)
	, myVertexShader(nullptr)
	, myInputLayout(nullptr)
{
	Reload();
}


CE_VertexShader::~CE_VertexShader()
{
	CE_SAFE_RELEASE(myInputLayout);
	CE_SAFE_RELEASE(myVertexShader);
}

void CE_VertexShader::Activate()
{
	ID3D11DeviceContext* context = myGPUContext.GetContext();
	context->IASetInputLayout(myInputLayout);
	context->VSSetShader(myVertexShader, NULL, 0);
}

void CE_VertexShader::Reload()
{
	ID3D10Blob* shaderBlob = CompileShader("vs_5_0");
	if (!shaderBlob)
		return;

	CE_SAFE_RELEASE(myInputLayout);
	CE_SAFE_RELEASE(myVertexShader);

	ID3D11Device* device = myGPUContext.GetDevice();
	HRESULT result = device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &myVertexShader);

	CE_GrowingArray<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	GetInputParamaters(vertexLayout);

	result = device->CreateInputLayout(vertexLayout.GetArrayAsPointer(), static_cast<unsigned int>(vertexLayout.Size()), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &myInputLayout);
	CE_ASSERT(FAILED(result) == false, "Failed to create inputlayout");

	CE_SAFE_RELEASE(shaderBlob);
}
