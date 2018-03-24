#pragma once

class CE_Texture;
class CE_GBuffer
{
public:
	enum eTextures
	{
		ALBEDO_METALNESS,
		NORMAL_ROUGNESS,
		DEPTH,
		__COUNT,
	};

	CE_GBuffer(CE_GPUContext& aGPUContext, const CE_Vector2i& aWindowSize);
	~CE_GBuffer();

	void SendToGPU(CE_GPUContext& aGPUContext);
	void Clear(CE_GPUContext& aGPUContext, const CE_Vector4f& aClearColor);
	CE_Texture* myTextures[__COUNT];
};