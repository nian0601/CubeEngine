#pragma once

struct ID3D11Buffer;

class CE_GPUContext;
class CE_Font;

class CE_Text
{
public:
	CE_Text();
	~CE_Text();

	void Init(const CE_GPUContext& aGPUContext);

	void Render(const CE_GPUContext& aGPUContext);

	int GetIndexCount() const { return myIndexCount; }
	const CE_Matrix44f& GetOrientation() const { return myOrientation; }

	void SetColor(const CE_Vector4f& aColor) { myColor = aColor; }
	void SetPosition(const CE_Vector2f& aPosition) { myPosition = aPosition; }
	void SetSize(const CE_Vector2f& aSize) { mySize = aSize; }

private:
	void SetupObjectBuffer(const CE_GPUContext& aGPUContext);
	void SetupVertexAndIndexBuffers(const CE_GPUContext& aGPUContext, void* aVertexData, void* aIndexData);

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

	CE_Font* myFont;
};

