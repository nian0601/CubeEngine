#pragma once
#include <wtypes.h>
#include <d3d11.h>
#include "CE_GPUContext.h"

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;
struct ID3D10Blob;

class CE_GPUContext;
class CE_Camera;


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

	template <typename T>
	void InitGlobalData();

	template <typename T>
	T* GetGlobalData();

private:
	void OutputError(ID3D10Blob* aErrorBlob, const WCHAR* aShaderName);

	CE_GPUContext& myGPUContext;

	ID3D11VertexShader* myVertexShader;
	ID3D11PixelShader* myPixelShader;
	ID3D11InputLayout* myInputLayout;
	ID3D11Buffer* myGlobalDataBuffer;
	D3D11_MAPPED_SUBRESOURCE myGlobalDataResource;
};

template <typename T>
void CE_Shader::InitGlobalData()
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(T);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	ID3D11Device* device = myGPUContext.GetDevice();
	HRESULT result = device->CreateBuffer(&matrixBufferDesc, NULL, &myGlobalDataBuffer);
	CE_ASSERT(FAILED(result) == false, "Failed to create GlobalDataBuffer");
}

template <typename T>
T* CE_Shader::GetGlobalData()
{
	CE_ASSERT(myGlobalDataBuffer != nullptr, "No GlobalData created for this shader");
	ID3D11DeviceContext* context = myGPUContext.GetContext();
	context->Map(myGlobalDataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &myGlobalDataResource);
	return (T*)myGlobalDataResource.pData;
}