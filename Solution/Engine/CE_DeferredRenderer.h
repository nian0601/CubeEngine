#pragma once

class CE_Camera;
class CE_GBuffer;
class CE_GPUContext;
class CE_Texture;

class CE_FullscreenQuad;
class CE_RenderObject;
class CE_Renderer;

class CE_ConstantBuffer;
class CE_ShaderPair;

class CE_DeferredRenderer
{
public:
	CE_DeferredRenderer(CE_GPUContext& aGPUContext, const CE_Vector2i& aWindowSize);
	~CE_DeferredRenderer();

	void SetBackbuffer(CE_Texture* aBackbuffer) { myBackbuffer = aBackbuffer; }

	void UpdateConstantBuffers(const CE_Camera& aCamera);
	void Render(CE_Renderer& aRenderer, const CE_RendererProxy& aRendererProxy);

private:
	void RenderPointLights(const CE_RendererProxy& aRendererProxy);
	void RenderToScreen();

	void BeginGBuffer();
	void EndGBuffer();

	CE_GPUContext& myGPUContext;
	CE_GBuffer* myGBuffer;
	CE_ConstantBuffer* myDefferedConstantBuffer;

	CE_ShaderPair* myFullscreenShader;
	CE_RenderObject* myQuad;

	CE_Texture* myCubeMap;
	CE_Texture* mySSAORandomTexture;

	CE_RenderObject* myPointLightModel;
	CE_ShaderPair* myPointLightShader;

	CE_Texture* myBackbuffer;
};

