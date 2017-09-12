#pragma once

struct ID3D11Buffer;

class CE_GPUContext;

class CE_Model
{
public:
	CE_Model();
	~CE_Model();

	void InitTriangle(const CE_GPUContext& aGPUContext);
	void InitCube(const CE_GPUContext& aGPUContext);
	void Render(const CE_GPUContext& aGPUContext);

	int GetIndexCount();

private:
	void InitBuffers(const CE_GPUContext& aGPUContext, void* aVertexData, void* aIndexData);

	struct VertexType
	{
		CE_Vector3f myPosition;
		CE_Vector3f myNormal;
		CE_Vector4f myColor;
	};

	ID3D11Buffer* myVertexBuffer;
	int myVertexCount;

	ID3D11Buffer* myIndexBuffer;
	int myIndexCount;
};

