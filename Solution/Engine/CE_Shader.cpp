#include "stdafx.h"

#include "CE_Camera.h"
#include "CE_GPUContext.h"
#include "CE_Shader.h"

#include <d3d11.h>
#include <d3dcompiler.h>

CE_Shader::CE_Shader(const CE_ShaderParameters& someParameters, CE_GPUContext& aGPUContext)
	: myGPUContext(aGPUContext)
	, myVertexShader(nullptr)
	, myPixelShader(nullptr)
	, myInputLayout(nullptr)
	, myGlobalDataBuffer(nullptr)
{
	HRESULT result;
	ID3D10Blob* errorMessage = nullptr;


	int compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3D10_SHADER_ENABLE_STRICTNESS;

	ID3D10Blob* vertexShaderBuffer = nullptr;
	result = D3DCompileFromFile(someParameters.myFilePath, NULL, NULL, "VS", "vs_5_0", compileFlags, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
			OutputError(errorMessage, someParameters.myFilePath);

		if (!errorMessage)
			CE_ASSERT_ALWAYS("Couldnt find shader %s", someParameters.myFilePath);
	}

	ID3D10Blob* pixelShaderBuffer = nullptr;
	result = D3DCompileFromFile(someParameters.myFilePath, NULL, NULL, "PS", "ps_5_0", compileFlags, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
			OutputError(errorMessage, someParameters.myFilePath);

		if (!errorMessage)
			CE_ASSERT_ALWAYS("Couldnt find shader %s", someParameters.myFilePath);
	}


	ID3D11Device* device = myGPUContext.GetDevice();
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &myVertexShader);
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &myPixelShader);


	int numElements = someParameters.myInputElements.Size();
	D3D11_INPUT_ELEMENT_DESC* vertexLayout = new D3D11_INPUT_ELEMENT_DESC[numElements];
	for (int i = 0; i < numElements; ++i)
	{
		vertexLayout[i].SemanticIndex = 0;
		vertexLayout[i].InputSlot = 0;
		vertexLayout[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		vertexLayout[i].InstanceDataStepRate = 0;
		vertexLayout[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		switch (someParameters.myInputElements[i])
		{
		case CE_ShaderParameters::POSITION:
			vertexLayout[i].SemanticName = "POSITION";
			vertexLayout[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		case CE_ShaderParameters::NORMAL:
			vertexLayout[i].SemanticName = "NORMAL";
			vertexLayout[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		case CE_ShaderParameters::COLOR:
			vertexLayout[i].SemanticName = "COLOR";
			vertexLayout[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		case CE_ShaderParameters::UV:
			vertexLayout[i].SemanticName = "TEXCOORD";
			vertexLayout[i].Format = DXGI_FORMAT_R32G32_FLOAT;
			break;
		default:
			CE_ASSERT_ALWAYS("Invalid InputElement");
			break;
		}
	}

	result = device->CreateInputLayout(vertexLayout, static_cast<unsigned int>(numElements), vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &myInputLayout);
	CE_ASSERT(FAILED(result) == false, "Failed to create inputlayout");

	CE_SAFE_DELETE_ARRAY(vertexLayout);
	CE_SAFE_RELEASE(vertexShaderBuffer);
	CE_SAFE_RELEASE(pixelShaderBuffer);
}


CE_Shader::~CE_Shader()
{
	CE_SAFE_RELEASE(myGlobalDataBuffer);
	CE_SAFE_RELEASE(myInputLayout);
	CE_SAFE_RELEASE(myPixelShader);
	CE_SAFE_RELEASE(myVertexShader);
}

void CE_Shader::Activate()
{
	ID3D11DeviceContext* context = myGPUContext.GetContext();

	if (myGlobalDataBuffer)
	{
		context->Unmap(myGlobalDataBuffer, 0);
		context->VSSetConstantBuffers(0, 1, &myGlobalDataBuffer);
	}

	context->IASetInputLayout(myInputLayout);

	context->VSSetShader(myVertexShader, NULL, 0);
	context->PSSetShader(myPixelShader, NULL, 0);
}

void CE_Shader::OutputError(ID3D10Blob* aErrorBlob, const WCHAR* aShaderName)
{
	const char* errorMsg = (const char*)(aErrorBlob->GetBufferPointer());
	CE_ASSERT_ALWAYS("Shader Error (%s): %s", aShaderName, errorMsg);

	CE_SAFE_RELEASE(aErrorBlob);
}
