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

	int GetIndexCount() const { return myIndexCount; }
	const CE_Matrix44f& GetOrientation() const { return myOrientation; }

	void SetOrientation(const CE_Matrix44f& anOrientation);
	void SetPosition(const CE_Vector3f& aPosition);
	void Rotate(const CE_Matrix44f& aRotation);
	void SetScale(const CE_Vector3f& aScale);
	void SetColor(const CE_Vector4f& aColor);

private:
	void InitBuffers(const CE_GPUContext& aGPUContext, void* aVertexData, void* aIndexData);

	struct VertexType
	{
		CE_Vector3f myPosition;
		CE_Vector3f myNormal;
		CE_Vector4f myColor;
	};

	struct ObjectData
	{
		CE_Matrix44f myWorld;
		CE_Vector4f myColor;
		CE_Vector4f myScale;
	};
	ID3D11Buffer* myObjectDataBuffer;

	ID3D11Buffer* myVertexBuffer;
	int myVertexCount;

	ID3D11Buffer* myIndexBuffer;
	int myIndexCount;

	CE_Matrix44f myOrientation;
	CE_Vector3f myScale;
	CE_Vector4f myColor;
};

