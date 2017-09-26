#pragma once

struct ID3D11Buffer;
class CE_GPUContext;


class CE_Sprite
{
public:
	CE_Sprite();
	~CE_Sprite();

	void Init(const CE_GPUContext& aGPUContext);

	void Render(const CE_GPUContext& aGPUContext);

	int GetIndexCount() const { return myIndexCount; }
	const CE_Matrix44f& GetOrientation() const { return myOrientation; }

	void SetColor(const CE_Vector4f& aColor);
	void SetPosition(const CE_Vector2f& aPosition);
	void SetSize(const CE_Vector2f& aSize);
	void SetHotspot(const CE_Vector2f& aHotspot);

private:
	void InitBuffers(const CE_GPUContext& aGPUContext, void* aVertexData, void* aIndexData);

	struct VertexType
	{
		CE_Vector3f myPosition;
	};

	struct ObjectData
	{
		CE_Matrix44f myWorld;
		CE_Vector4f myColor;
		CE_Vector2f myPosition;
		CE_Vector2f mySize;
		CE_Vector2f myHotspot;
		CE_Vector2f padding;
	};
	ID3D11Buffer* myObjectDataBuffer;

	ID3D11Buffer* myVertexBuffer;
	int myVertexCount;

	ID3D11Buffer* myIndexBuffer;
	int myIndexCount;

	CE_Matrix44f myOrientation;
	CE_Vector4f myColor;
	CE_Vector2f myPosition;
	CE_Vector2f mySize;
	CE_Vector2f myHotspot;
};

