#pragma once

#include "CE_GenericShader.h"

struct ID3D11PixelShader;

class CE_PixelShader : public CE_GenericShader
{
public:
	CE_PixelShader(const char* aFilePath, CE_GPUContext& aGPUContext);
	~CE_PixelShader();

	void Activate() override;
	void Reload() override;

private:
	ID3D11PixelShader* myPiexlShader;
};