#pragma once

struct ID3D11Buffer;

class CE_GBuffer;
class CE_GPUContext;


class CE_FullscreenQuad
{
public:
	CE_FullscreenQuad();
	~CE_FullscreenQuad();

	void Init(const CE_GPUContext& aGPUContext);
	void Render(const CE_GPUContext& aGPUContext, const CE_GBuffer& aGBuffer);

private:
	void InitBuffers(const CE_GPUContext& aGPUContext, void* aVertexData, void* aIndexData);

	struct VertexType
	{
		CE_Vector3f myPosition;
		CE_Vector2f myTexCoord;
	};

	ID3D11Buffer* myVertexBuffer;
	int myVertexCount;

	ID3D11Buffer* myIndexBuffer;
	int myIndexCount;
};

