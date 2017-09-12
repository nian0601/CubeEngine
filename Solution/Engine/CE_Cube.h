#pragma once

struct ID3D11Buffer;

class CE_GPUContext;

class CE_Cube
{
public:
	CE_Cube();
	~CE_Cube();

	void Init(const CE_GPUContext& aGPUContext);
	void Render(const CE_GPUContext& aGPUContext);

	int GetIndexCount();

private:
	struct VertexType
	{
		CE_Vector3f myPosition;
		CE_Vector4f myColor;
	};

	ID3D11Buffer* myVertexBuffer;
	int myVertexCount;

	ID3D11Buffer* myIndexBuffer;
	int myIndexCount;
};

