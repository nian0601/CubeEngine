#include "stdafx.h"
#include "CE_Shader.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "CE_GPUContext.h"


CE_Shader::CE_Shader()
	: myVertexShader(nullptr)
	, myPixelShader(nullptr)
	, myInputLayout(nullptr)
	, myMatrixBuffer(nullptr)
{
}


CE_Shader::~CE_Shader()
{
}

void CE_Shader::Init(const WCHAR* aShaderFilePath, const CE_GPUContext& aGPUContext)
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

	D3D11_INPUT_ELEMENT_DESC vertexLayout[3];
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

	unsigned int numElements = sizeof(vertexLayout) / sizeof(vertexLayout[0]);

	result = device->CreateInputLayout(vertexLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &myInputLayout);

	CE_SAFE_RELEASE(vertexShaderBuffer);
	CE_SAFE_RELEASE(pixelShaderBuffer);

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, NULL, &myMatrixBuffer);
}

void CE_Shader::Shutdown()
{
	CE_SAFE_RELEASE(myMatrixBuffer);
	CE_SAFE_RELEASE(myInputLayout);
	CE_SAFE_RELEASE(myPixelShader);
	CE_SAFE_RELEASE(myVertexShader);
}

void CE_Shader::Render(const CE_GPUContext& aGPUContext, int aIndexCount, const CE_Matrix44f& aWorld, const CE_Matrix44f& aView, const CE_Matrix44f& aProjection)
{
	ID3D11DeviceContext* context = aGPUContext.GetContext();

	//Update constant-buffers
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	context->Map(myMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->myWorld = aWorld;
	dataPtr->myView = aView;
	dataPtr->myProjection = aProjection;

	context->Unmap(myMatrixBuffer, 0);

	context->VSSetConstantBuffers(0, 1, &myMatrixBuffer);



	//Actual draw-call
	context->IASetInputLayout(myInputLayout);

	context->VSSetShader(myVertexShader, NULL, 0);
	context->PSSetShader(myPixelShader, NULL, 0);

	context->DrawIndexed(aIndexCount, 0, 0);
}

void CE_Shader::OutputError(ID3D10Blob* aErrorBlob, const WCHAR* aShaderName)
{
	const char* errorMsg = (const char*)(aErrorBlob->GetBufferPointer());
	CE_ASSERT_ALWAYS("Shader Error (%s): %s", aShaderName, errorMsg);

	CE_SAFE_RELEASE(aErrorBlob);
}
