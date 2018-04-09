#pragma once

class CE_Camera;
class CE_GBuffer;
class CE_GPUContext;
class CE_Texture;

class CE_Shader;
class CE_FullscreenQuad;
class CE_RenderObject;

class CE_DeferredRenderer
{
public:
	CE_DeferredRenderer(CE_GPUContext& aGPUContext, const CE_Vector2i& aWindowSize);
	~CE_DeferredRenderer();

	void BeginGBuffer(CE_Texture* aBackbuffer);
	void EndGBuffer(CE_Texture* aBackbuffer);

	void RenderPointLights(const CE_Camera& aCamera, const CE_RendererProxy& aRendererProxy);
	void RenderToScreen(CE_Camera* aCamera);

private:
	CE_GBuffer* myGBuffer;
	CE_GPUContext& myGPUContext;

	CE_Shader* myShader;
	CE_RenderObject* myQuad;

	CE_Texture* myCubeMap;

	CE_RenderObject* myPointLightModel;
	CE_Shader* myPointLightShader;
};

