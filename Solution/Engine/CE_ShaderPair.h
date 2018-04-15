#pragma once

class CE_VertexShader;
class CE_PixelShader;
class CE_GenericShader;

class CE_ShaderPair
{
public:
	CE_ShaderPair(const char* aVertexShader, const char* aPixelShader, CE_GPUContext& aGPUContext);
	CE_ShaderPair(CE_GenericShader* aVertexShader, CE_GenericShader* aPixelShader);
	~CE_ShaderPair();

	void Activate();

private:
	CE_GenericShader* myVertexShader;
	CE_GenericShader* myPixelShader;
};

