#include "stdafx.h"

#include "CE_Camera.h"
#include "CE_GPUContext.h"
#include "CE_Shader.h"

#include <d3d11.h>
#include <d3dcompiler.h>

namespace CE_Shader_priv
{
	DXGI_FORMAT GetFormat(const D3D11_SIGNATURE_PARAMETER_DESC& someParams)
	{
		if (someParams.Mask == 1)
		{
			if (someParams.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return DXGI_FORMAT_R32_UINT;
			else if (someParams.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return DXGI_FORMAT_R32_SINT;
			else if (someParams.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return DXGI_FORMAT_R32_FLOAT;
		}
		else if (someParams.Mask <= 3)
		{
			if (someParams.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return DXGI_FORMAT_R32G32_UINT;
			else if (someParams.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return DXGI_FORMAT_R32G32_SINT;
			else if (someParams.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (someParams.Mask <= 7)
		{
			if (someParams.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return DXGI_FORMAT_R32G32B32_UINT;
			else if (someParams.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return DXGI_FORMAT_R32G32B32_SINT;
			else if (someParams.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (someParams.Mask <= 15)
		{
			if (someParams.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return DXGI_FORMAT_R32G32B32A32_UINT;
			else if (someParams.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return DXGI_FORMAT_R32G32B32A32_SINT;
			else if (someParams.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		
		CE_ASSERT_ALWAYS("Failed to GetFormat");
		return DXGI_FORMAT_R32_UINT;
	}
}

CE_Shader::CE_Shader(const WCHAR* aFilePath, CE_GPUContext& aGPUContext)
	: myGPUContext(aGPUContext)
	, myVertexShader(nullptr)
	, myPixelShader(nullptr)
	, myInputLayout(nullptr)
{
	HRESULT result;
	ID3D10Blob* errorMessage = nullptr;


	int compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3D10_SHADER_ENABLE_STRICTNESS;

	ID3D10Blob* vertexShaderBuffer = nullptr;
	result = D3DCompileFromFile(aFilePath, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", compileFlags, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
			OutputError(errorMessage, aFilePath);

		if (!errorMessage)
			CE_ASSERT_ALWAYS("Couldnt find shader %s", aFilePath);
	}

	ID3D10Blob* pixelShaderBuffer = nullptr;
	result = D3DCompileFromFile(aFilePath, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", compileFlags, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
			OutputError(errorMessage, aFilePath);

		if (!errorMessage)
			CE_ASSERT_ALWAYS("Couldnt find shader %s", aFilePath);
	}


	ID3D11Device* device = myGPUContext.GetDevice();
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &myVertexShader);
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &myPixelShader);

	ID3D11ShaderReflection* reflection = nullptr; 
	result = D3DReflect(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflection);
	CE_ASSERT(SUCCEEDED(result), "Failed to Reflect vertex shader");

	D3D11_SHADER_DESC shaderDesc;
	reflection->GetDesc(&shaderDesc);

	int numElements = shaderDesc.InputParameters;
	D3D11_INPUT_ELEMENT_DESC* vertexLayout = new D3D11_INPUT_ELEMENT_DESC[numElements];
	for (int i = 0; i < numElements; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		reflection->GetInputParameterDesc(i, &paramDesc);

		vertexLayout[i].SemanticIndex = paramDesc.SemanticIndex;
		vertexLayout[i].SemanticName = paramDesc.SemanticName;
		vertexLayout[i].Format = CE_Shader_priv::GetFormat(paramDesc);
		

		vertexLayout[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		vertexLayout[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		vertexLayout[i].InputSlot = 0;
		vertexLayout[i].InstanceDataStepRate = 0;
	}

	result = device->CreateInputLayout(vertexLayout, static_cast<unsigned int>(numElements), vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &myInputLayout);
	CE_ASSERT(FAILED(result) == false, "Failed to create inputlayout");

	CE_SAFE_DELETE_ARRAY(vertexLayout);
	CE_SAFE_RELEASE(vertexShaderBuffer);
	CE_SAFE_RELEASE(pixelShaderBuffer);
}


CE_Shader::~CE_Shader()
{
	CE_SAFE_RELEASE(myInputLayout);
	CE_SAFE_RELEASE(myPixelShader);
	CE_SAFE_RELEASE(myVertexShader);
}

void CE_Shader::Activate()
{
	ID3D11DeviceContext* context = myGPUContext.GetContext();
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
