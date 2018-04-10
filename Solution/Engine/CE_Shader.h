#pragma once
#include <wtypes.h>
#include "CE_GPUContext.h"

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D10Blob;

class CE_GPUContext;


struct CE_ShaderParameters
{
	enum InputElement
	{
		POSITION,
		NORMAL,
		COLOR,
		UV,
	};

	CE_GrowingArray<InputElement> myInputElements;
	const WCHAR* myFilePath;
};

class CE_Shader
{
public:
	CE_Shader(const CE_ShaderParameters& someParameters, CE_GPUContext& aGPUContext);
	~CE_Shader();

	void Activate();

private:
	void OutputError(ID3D10Blob* aErrorBlob, const WCHAR* aShaderName);

	CE_GPUContext& myGPUContext;

	ID3D11VertexShader* myVertexShader;
	ID3D11PixelShader* myPixelShader;
	ID3D11InputLayout* myInputLayout;
};