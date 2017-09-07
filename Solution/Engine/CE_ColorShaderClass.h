#pragma once
#include <wtypes.h>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;

struct ID3D11Device;
struct ID3D11DeviceContext;

struct ID3D10Blob;

class CE_ColorShaderClass
{
public:
	CE_ColorShaderClass();
	~CE_ColorShaderClass();

	void Init(ID3D11Device* aDevice);
	void Shutdown();
	void Render(ID3D11DeviceContext* aContext, int aIndexCount, const CE_Matrix44f& aWorld, const CE_Matrix44f& aView, const CE_Matrix44f& aProjection);

private:
	void InitShader(ID3D11Device* aDevice, const WCHAR* aVSName, const WCHAR* aPSName);
	void SetShaderParameters(ID3D11DeviceContext* aContext, const CE_Matrix44f& aWorld, const CE_Matrix44f& aView, const CE_Matrix44f& aProjection);
	void RenderShader(ID3D11DeviceContext* aContext, int aIndexCount);
	void OutputError(ID3D10Blob* aErrorBlob, const WCHAR* aShaderName);

	struct MatrixBufferType
	{
		CE_Matrix44f myWorld;
		CE_Matrix44f myView;
		CE_Matrix44f myProjection;
	};

	ID3D11VertexShader* myVertexShader;
	ID3D11PixelShader* myPixelShader;
	ID3D11InputLayout* myInputLayout;
	ID3D11Buffer* myMatrixBuffer;
};