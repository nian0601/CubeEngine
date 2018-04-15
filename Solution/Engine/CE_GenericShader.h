#pragma once

#include "CE_ShaderSignature.h"
#include <d3d11.h>

struct ID3D11InputLayout;
struct ID3D10Blob;
struct ID3D11ShaderReflection;

class CE_GPUContext;
class CE_GenericShader
{
public:
	CE_GenericShader(const char* aFilePath, CE_GPUContext& aGPUContext);
	virtual ~CE_GenericShader();

	virtual void Activate() = 0;
	virtual void Reload() = 0;

	void VerifyVertexShader(const CE_GenericShader& aVertexShader)
	{
		CE_ASSERT(myInputSignature == aVertexShader.myOutputSignature, "Signatures dont match..");
	}

protected:
	ID3D10Blob* CompileShader(const char* aCompilerTarget);
	void GetInputParamaters(CE_GrowingArray<D3D11_INPUT_ELEMENT_DESC>& someElementsOut);

	void CompileInputSignature(CE_ShaderSignature& aSignatureOut);
	void CompileOutputSignature(CE_ShaderSignature& aSignatureOut);

	CE_String myFileName;
	CE_GPUContext& myGPUContext;

	ID3D11ShaderReflection* myShaderReflection;
	CE_ShaderSignature myInputSignature;
	CE_ShaderSignature myOutputSignature;
};

