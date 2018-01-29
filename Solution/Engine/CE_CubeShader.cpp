#include "stdafx.h"
#include "CE_CubeShader.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "CE_GPUContext.h"
#include "CE_Camera.h"
#include "CE_Model.h"


CE_CubeShader::CE_CubeShader()
	: myVertexShader(nullptr)
	, myPixelShader(nullptr)
	, myInputLayout(nullptr)
	, myGlobalDataBuffer(nullptr)
{
}


CE_CubeShader::~CE_CubeShader()
{
	CE_SAFE_RELEASE(myGlobalDataBuffer);
	CE_SAFE_RELEASE(myInputLayout);
	CE_SAFE_RELEASE(myPixelShader);
	CE_SAFE_RELEASE(myVertexShader);
}

void CE_CubeShader::Init(const WCHAR* aShaderFilePath, const CE_GPUContext& aGPUContext, bool aOnlyPosition)
{
	HRESULT result;
	ID3D10Blob* errorMessage = nullptr;


	int compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3D10_SHADER_ENABLE_STRICTNESS;

	ID3D10Blob* vertexShaderBuffer = nullptr;
	result = D3DCompileFromFile(aShaderFilePath, NULL, NULL, "VS", "vs_5_0", compileFlags, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
			OutputError(errorMessage, aShaderFilePath);

		if (!errorMessage)
			CE_ASSERT_ALWAYS("Couldnt find shader %s", aShaderFilePath);
	}

	ID3D10Blob* pixelShaderBuffer = nullptr;
	result = D3DCompileFromFile(aShaderFilePath, NULL, NULL, "PS", "ps_5_0", compileFlags, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
			OutputError(errorMessage, aShaderFilePath);

		if (!errorMessage)
			CE_ASSERT_ALWAYS("Couldnt find shader %s", aShaderFilePath);
	}


	ID3D11Device* device = aGPUContext.GetDevice();
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &myVertexShader);
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &myPixelShader);

	if (!aOnlyPosition)
	{
		const unsigned int numElements = 3;
		D3D11_INPUT_ELEMENT_DESC vertexLayout[numElements];

		vertexLayout[0].SemanticName = "POSITION";
		vertexLayout[0].SemanticIndex = 0;
		vertexLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		vertexLayout[0].InputSlot = 0;
		vertexLayout[0].AlignedByteOffset = 0;
		vertexLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		vertexLayout[0].InstanceDataStepRate = 0;

		vertexLayout[1].SemanticName = "NORMAL";
		vertexLayout[1].SemanticIndex = 0;
		vertexLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		vertexLayout[1].InputSlot = 0;
		vertexLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		vertexLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		vertexLayout[1].InstanceDataStepRate = 0;

		vertexLayout[2].SemanticName = "COLOR";
		vertexLayout[2].SemanticIndex = 0;
		vertexLayout[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		vertexLayout[2].InputSlot = 0;
		vertexLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		vertexLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		vertexLayout[2].InstanceDataStepRate = 0;

		result = device->CreateInputLayout(vertexLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &myInputLayout);
	}
	else
	{
		const unsigned int numElements = 2;
		D3D11_INPUT_ELEMENT_DESC vertexLayout[numElements];

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

		result = device->CreateInputLayout(vertexLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &myInputLayout);
	}

	CE_SAFE_RELEASE(vertexShaderBuffer);
	CE_SAFE_RELEASE(pixelShaderBuffer);

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(GlobalData);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, NULL, &myGlobalDataBuffer);
}

void CE_CubeShader::SetGlobalGPUData(const CE_GPUContext& aGPUContext, const CE_Camera& aCamera)
{
	ID3D11DeviceContext* context = aGPUContext.GetContext();

	//Update constant-buffers
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	context->Map(myGlobalDataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	GlobalData* dataPtr = (GlobalData*)mappedResource.pData;
	dataPtr->myView = aCamera.GetView();
	dataPtr->myProjection = aCamera.GetProjection();

	context->Unmap(myGlobalDataBuffer, 0);

	context->VSSetConstantBuffers(0, 1, &myGlobalDataBuffer);

	context->IASetInputLayout(myInputLayout);

	context->VSSetShader(myVertexShader, NULL, 0);
	context->PSSetShader(myPixelShader, NULL, 0);
}

void CE_CubeShader::OutputError(ID3D10Blob* aErrorBlob, const WCHAR* aShaderName)
{
	const char* errorMsg = (const char*)(aErrorBlob->GetBufferPointer());
	CE_ASSERT_ALWAYS("Shader Error (%s): %s", aShaderName, errorMsg);

	CE_SAFE_RELEASE(aErrorBlob);
}
