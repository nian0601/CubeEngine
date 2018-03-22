#pragma once

class CE_GBuffer;
class CE_GPUContext;
class CE_Texture;

class CE_Shader;
class CE_FullscreenQuad;

class CE_DeferredRenderer
{
public:
	CE_DeferredRenderer(CE_GPUContext& aGPUContext, const CE_Vector2i& aWindowSize);
	~CE_DeferredRenderer();

	void BeginGBuffer(CE_Texture* aBackbuffer);
	void EndGBuffer(CE_Texture* aBackbuffer);

	void RenderToScreen();

private:
	CE_GBuffer* myGBuffer;
	CE_GPUContext& myGPUContext;

	CE_Shader* myShader;
	CE_FullscreenQuad* myQuad;
};

