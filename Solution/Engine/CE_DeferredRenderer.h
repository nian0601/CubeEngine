#pragma once

class CE_GPUContext;
class CE_Texture;
class CE_DeferredRenderer
{
public:
	CE_DeferredRenderer(CE_GPUContext& aGPUContext, const CE_Vector2i& aWindowSize);
	~CE_DeferredRenderer();

	void BeginGBuffer();
	void EndGBuffer();

	void RenderToScreen();

private:
	struct GBuffer
	{
		enum eTextures
		{
			ALBEDO_METALNESS,
			NORMAL_ROUGNESS,
			DEPTH,
			DEPTH_STENCIL,
			__COUNT,
		};

		GBuffer(CE_GPUContext& aGPUContext, const CE_Vector2i& aWindowSize);
		~GBuffer();

		void Clear(CE_GPUContext& aGPUContext, const CE_Vector4f& aClearColor);
		CE_Texture* myTextures[__COUNT];
	};
	
	GBuffer* myGBuffer;
	CE_GPUContext& myGPUContext;
};

