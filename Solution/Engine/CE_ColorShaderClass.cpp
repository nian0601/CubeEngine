#include "stdafx.h"
#include "CE_ColorShaderClass.h"
#include <d3d11.h>
#include <d3dcompiler.h>


CE_ColorShaderClass::CE_ColorShaderClass()
	: myVertexShader(nullptr)
	, myPixelShader(nullptr)
	, myInputLayout(nullptr)
	, myMatrixBuffer(nullptr)
{
}


CE_ColorShaderClass::~CE_ColorShaderClass()
{
}

void CE_ColorShaderClass::Init(ID3D11Device* aDevice)
{
	InitShader(aDevice, L"Data/Shaders/Color.vs", L"Data/Shaders/Color.ps");
}

void CE_ColorShaderClass::Shutdown()
{
	CE_SAFE_RELEASE(myMatrixBuffer);
	CE_SAFE_RELEASE(myInputLayout);
	CE_SAFE_RELEASE(myPixelShader);
	CE_SAFE_RELEASE(myVertexShader);
}

void CE_ColorShaderClass::Render(ID3D11DeviceContext* aContext, int aIndexCount, const CE_Matrix44f& aWorld, const CE_Matrix44f& aViewProjection)
{
	SetShaderParameters(aContext, aWorld, aViewProjection);

	RenderShader(aContext, aIndexCount);
}

void CE_ColorShaderClass::InitShader(ID3D11Device* aDevice, const WCHAR* aVSName, const WCHAR* aPSName)
{
	HRESULT result;
	ID3D10Blob* errorMessage = nullptr;

	
	int compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3D10_SHADER_ENABLE_STRICTNESS;

	ID3D10Blob* vertexShaderBuffer = nullptr;
	result = D3DCompileFromFile(aVSName, NULL, NULL, "ColorVertexShader", "vs_5_0", compileFlags, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
			OutputError(errorMessage, aVSName);

		if (!errorMessage)
			CE_ASSERT_ALWAYS("Couldnt find shader %s", aVSName);
	}

	ID3D10Blob* pixelShaderBuffer = nullptr;
	result = D3DCompileFromFile(aPSName, NULL, NULL, "ColorPixelShader", "ps_5_0", compileFlags, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
			OutputError(errorMessage, aPSName);

		if (!errorMessage)
			CE_ASSERT_ALWAYS("Couldnt find shader %s", aPSName);
	}

	result = aDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &myVertexShader);
	result = aDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &myPixelShader);

	D3D11_INPUT_ELEMENT_DESC vertexLayout[2];
	vertexLayout[0].SemanticName = "POSITION";
	vertexLayout[0].SemanticIndex = 0;
	vertexLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexLayout[0].InputSlot = 0;
	vertexLayout[0].AlignedByteOffset = 0;
	vertexLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout[0].InstanceDataStepRate = 0;

	vertexLayout[1].SemanticName = "COLOR";
	vertexLayout[1].SemanticIndex = 0;
	vertexLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	vertexLayout[1].InputSlot = 0;
	vertexLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vertexLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout[1].InstanceDataStepRate = 0;

	unsigned int numElements = sizeof(vertexLayout) / sizeof(vertexLayout[0]);

	result = aDevice->CreateInputLayout(vertexLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &myInputLayout);

	CE_SAFE_RELEASE(vertexShaderBuffer);
	CE_SAFE_RELEASE(pixelShaderBuffer);

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = aDevice->CreateBuffer(&matrixBufferDesc, NULL, &myMatrixBuffer);
}

void CE_ColorShaderClass::SetShaderParameters(ID3D11DeviceContext* aContext, const CE_Matrix44f& aWorld, const CE_Matrix44f& aViewProjection)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	aContext->Map(myMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->myWorld = aWorld;
	dataPtr->myViewProjection = aViewProjection;

	aContext->Unmap(myMatrixBuffer, 0);

	aContext->VSSetConstantBuffers(0, 1, &myMatrixBuffer);
}

void CE_ColorShaderClass::RenderShader(ID3D11DeviceContext* aContext, int aIndexCount)
{
	aContext->IASetInputLayout(myInputLayout);

	aContext->VSSetShader(myVertexShader, NULL, 0);
	aContext->PSSetShader(myPixelShader, NULL, 0);

	aContext->DrawIndexed(aIndexCount, 0, 0);
}

void CE_ColorShaderClass::OutputError(ID3D10Blob* aErrorBlob, const WCHAR* aShaderName)
{
	const char* errorMsg = (const char*)(aErrorBlob->GetBufferPointer());
	CE_ASSERT_ALWAYS("Shader Error (%s): %s", aShaderName, errorMsg);

	CE_SAFE_RELEASE(aErrorBlob);
}
