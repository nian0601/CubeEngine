#include "stdafx.h"

#include "CE_Camera.h"
#include "CE_GPUContext.h"
#include "CE_PixelShader.h"

#include <d3d11.h>
#include <d3dcompiler.h>

#include "CE_GPUContext.h"
#include "CE_VertexShader.h"

CE_PixelShader::CE_PixelShader(const char* aFilePath, CE_GPUContext& aGPUContext)
	: CE_GenericShader(aFilePath, aGPUContext)
	, myPiexlShader(nullptr)
{
	ID3D10Blob* shaderBlob = CompileShader("ps_5_0");
	myGPUContext.GetDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &myPiexlShader);

	CE_GrowingArray<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	GetInputParamaters(vertexLayout);

	CE_SAFE_RELEASE(shaderBlob);
}


CE_PixelShader::~CE_PixelShader()
{
	CE_SAFE_RELEASE(myPiexlShader);
}

void CE_PixelShader::Activate()
{
	ID3D11DeviceContext* context = myGPUContext.GetContext();
	context->PSSetShader(myPiexlShader, NULL, 0);
}
