#pragma once

class CE_VertexShader;
class CE_PixelShader;

class CE_ShaderPair
{
public:
	CE_ShaderPair(const char* aVertexShader, const char* aPixelShader, CE_GPUContext& aGPUContext);
	~CE_ShaderPair();

	void Activate();

private:
	CE_VertexShader* myVertexShader;
	CE_PixelShader* myPixelShader;
};

