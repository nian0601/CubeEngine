#pragma once
#include <wtypes.h>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;
struct ID3D11SamplerState;
struct ID3D10Blob;

class CE_GPUContext;
class CE_Camera;

class CE_TextShader
{
public:
	CE_TextShader();
	~CE_TextShader();

	void Init(const WCHAR* aShaderFilePath, const CE_GPUContext& aGPUContext);

	void SetGlobalGPUData(const CE_GPUContext& aGPUContext, const CE_Camera& aCamera);

private:
	void OutputError(ID3D10Blob* aErrorBlob, const WCHAR* aShaderName);

	struct GlobalData
	{
		CE_Matrix44f myProjection;
	};

	ID3D11VertexShader* myVertexShader;
	ID3D11PixelShader* myPixelShader;
	ID3D11InputLayout* myInputLayout;
	ID3D11Buffer* myGlobalDataBuffer;
	ID3D11SamplerState* mySamplerState;
};

