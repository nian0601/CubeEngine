#pragma once

#include "CE_GenericShader.h"


struct ID3D11VertexShader;

class CE_VertexShader : public CE_GenericShader
{
public:
	CE_VertexShader(const char* aFilePath, CE_GPUContext& aGPUContext);
	~CE_VertexShader();

	void Activate();

private:
	ID3D11VertexShader* myVertexShader;
	ID3D11InputLayout* myInputLayout;
};