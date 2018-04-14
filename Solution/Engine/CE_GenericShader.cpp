#include "stdafx.h"

#include "CE_GenericShader.h"

#include <d3d11.h>
#include <d3dcompiler.h>

namespace CE_GenericShader_priv
{
	int GetParameterType(const D3D11_SIGNATURE_PARAMETER_DESC& someParams)
	{
		if (someParams.Mask == 1)
		{
			if (someParams.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return 0; //DXGI_FORMAT_R32_UINT;
			else if (someParams.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return 1; //DXGI_FORMAT_R32_SINT;
			else if (someParams.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return 2; //DXGI_FORMAT_R32_FLOAT;
		}
		else if (someParams.Mask <= 3)
		{
			if (someParams.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return 3; //DXGI_FORMAT_R32G32_UINT;
			else if (someParams.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return 4; //DXGI_FORMAT_R32G32_SINT;
			else if (someParams.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return 5; //DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (someParams.Mask <= 7)
		{
			if (someParams.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return 6; //DXGI_FORMAT_R32G32B32_UINT;
			else if (someParams.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return 7; //DXGI_FORMAT_R32G32B32_SINT;
			else if (someParams.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return 8; //DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (someParams.Mask <= 15)
		{
			if (someParams.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return 9; //DXGI_FORMAT_R32G32B32A32_UINT;
			else if (someParams.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return 10; //DXGI_FORMAT_R32G32B32A32_SINT;
			else if (someParams.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return 11; //DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		CE_ASSERT_ALWAYS("Failed to GetParameterType");
		return -1;
	}

	DXGI_FORMAT GetParameterFormat(int aType)
	{
		switch (aType)
		{
		case 0: return DXGI_FORMAT_R32_UINT;
		case 1: return DXGI_FORMAT_R32_SINT;
		case 2: return DXGI_FORMAT_R32_FLOAT;

		case 3: return DXGI_FORMAT_R32G32_UINT;
		case 4: return DXGI_FORMAT_R32G32_SINT;
		case 5: return DXGI_FORMAT_R32G32_FLOAT;

		case 6: return DXGI_FORMAT_R32G32B32_UINT;
		case 7: return DXGI_FORMAT_R32G32B32_SINT;
		case 8: return DXGI_FORMAT_R32G32B32_FLOAT;

		case 9: return DXGI_FORMAT_R32G32B32A32_UINT;
		case 10: return DXGI_FORMAT_R32G32B32A32_SINT;
		case 11: return DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		CE_ASSERT_ALWAYS("Failed to GetParameterFormat");
		return DXGI_FORMAT_R32_UINT;
	}

	static CE_String POSITION("POSITION");
	static CE_String NORMAL("NORMAL");
	static CE_String COLOR("COLOR");
	static CE_String TEXCOORD("TEXCOORD");
	static CE_String SV_POSITION("SV_POSITION");
	static CE_String SV_TARGET("SV_TARGET");

	int GetSemanticNameIndex(const char* aName)
	{
		if (POSITION == aName) return 0;
		if (NORMAL == aName) return 1;
		if (COLOR == aName) return 2;
		if (TEXCOORD == aName) return 3;
		if (SV_POSITION == aName) return 4;
		if (SV_TARGET == aName) return 5;

		CE_ASSERT_ALWAYS("Unknown SemanticName %s", aName);
		return -1;
	}

	const char* GetSemanticName(int aIndex)
	{
		switch (aIndex)
		{
		case 0: return POSITION.c_str();
		case 1: return NORMAL.c_str();
		case 2: return COLOR.c_str();
		case 3: return TEXCOORD.c_str();
		case 4: return SV_POSITION.c_str();
		case 5: return SV_TARGET.c_str();
		}

		CE_ASSERT_ALWAYS("Unknown SemanticNameIndex %d", aIndex);
		return "Invalid";
	}

	CE_SignatureElement GetSignatureElement(const D3D11_SIGNATURE_PARAMETER_DESC& someParams)
	{
		CE_SignatureElement element;
		element.myParameterType = GetParameterType(someParams);
		element.mySemanticIndex = someParams.SemanticIndex;
		element.mySemanticNameIndex = GetSemanticNameIndex(someParams.SemanticName);

		return element;
	}
}

CE_GenericShader::CE_GenericShader(const char* aFilePath, CE_GPUContext& aGPUContext)
	: myGPUContext(aGPUContext)
	, myFileName(aFilePath)
	, myShaderReflection(nullptr)
{
}


CE_GenericShader::~CE_GenericShader()
{
	CE_SAFE_RELEASE(myShaderReflection);
}

ID3D10Blob* CE_GenericShader::CompileShader(const char* aCompilerTarget)
{
	HRESULT result;
	ID3D10Blob* errorMessage = nullptr;

	int compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3D10_SHADER_ENABLE_STRICTNESS;

	size_t lenght = myFileName.Lenght()+2;
	wchar_t* buf = new wchar_t[lenght];
	const char* name = myFileName.c_str();
	mbsrtowcs(buf, &name, lenght, NULL);
	buf[lenght-1] = '\0';

	ID3D10Blob* shaderBlob = nullptr;
	result = D3DCompileFromFile(buf, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", aCompilerTarget, compileFlags, 0, &shaderBlob, &errorMessage);
	CE_SAFE_DELETE_ARRAY(buf);
	if (FAILED(result))
	{
		if (errorMessage)
			OutputError(errorMessage);

		if (!errorMessage)
			CE_ASSERT_ALWAYS("Couldnt find shader %s", myFileName.c_str());
	}

	result = D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&myShaderReflection);
	CE_ASSERT(SUCCEEDED(result), "Failed to Reflect shader");

	CompileInputSignature(myInputSignature);
	CompileOutputSignature(myOutputSignature);

	return shaderBlob;
}

void CE_GenericShader::GetInputParamaters(CE_GrowingArray<D3D11_INPUT_ELEMENT_DESC>& someElementsOut)
{
	for (const CE_SignatureElement& element : myInputSignature.myElements)
	{
		D3D11_INPUT_ELEMENT_DESC& elem = someElementsOut.Add();
		elem.SemanticIndex = element.mySemanticIndex;
		elem.SemanticName = CE_GenericShader_priv::GetSemanticName(element.mySemanticNameIndex);
		elem.Format = CE_GenericShader_priv::GetParameterFormat(element.myParameterType);

		elem.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elem.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elem.InputSlot = 0;
		elem.InstanceDataStepRate = 0;
	}
}

void CE_GenericShader::CompileInputSignature(CE_ShaderSignature& aSignatureOut)
{
	D3D11_SHADER_DESC shaderDesc;
	myShaderReflection->GetDesc(&shaderDesc);

	for (unsigned int i = 0; i < shaderDesc.InputParameters; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		myShaderReflection->GetInputParameterDesc(i, &paramDesc);

		CE_SignatureElement& element = aSignatureOut.myElements.Add();
		element = CE_GenericShader_priv::GetSignatureElement(paramDesc);
	}
}

void CE_GenericShader::CompileOutputSignature(CE_ShaderSignature& aSignatureOut)
{
	D3D11_SHADER_DESC shaderDesc;
	myShaderReflection->GetDesc(&shaderDesc);

	for (unsigned int i = 0; i < shaderDesc.OutputParameters; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		myShaderReflection->GetOutputParameterDesc(i, &paramDesc);

		CE_SignatureElement& element = aSignatureOut.myElements.Add();
		element = CE_GenericShader_priv::GetSignatureElement(paramDesc);
	}
}

void CE_GenericShader::OutputError(ID3D10Blob* aErrorBlob)
{
	const char* errorMsg = (const char*)(aErrorBlob->GetBufferPointer());
	CE_ASSERT_ALWAYS("Shader Error (%s): %s", myFileName.c_str(), errorMsg);

	CE_SAFE_RELEASE(aErrorBlob);
}
