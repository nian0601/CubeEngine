#include "stdafx.h"
#include "CE_ShaderPair.h"
#include "CE_VertexShader.h"
#include "CE_PixelShader.h"

CE_ShaderPair::CE_ShaderPair(const char* aVertexShader, const char* aPixelShader, CE_GPUContext& aGPUContext)
{
	myVertexShader = new CE_VertexShader(aVertexShader, aGPUContext);
	myPixelShader = new CE_PixelShader(aPixelShader, aGPUContext);

	myPixelShader->VerifyVertexShader(*myVertexShader);
}

CE_ShaderPair::CE_ShaderPair(CE_GenericShader* aVertexShader, CE_GenericShader* aPixelShader)
	: myVertexShader(aVertexShader)
	, myPixelShader(aPixelShader)
{
	myPixelShader->VerifyVertexShader(*myVertexShader);
}

CE_ShaderPair::~CE_ShaderPair()
{
}

void CE_ShaderPair::Activate()
{
	myVertexShader->Activate();
	myPixelShader->Activate();
}
