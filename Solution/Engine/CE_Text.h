#pragma once

struct ID3D11Buffer;

class CE_GPUContext;
class CE_Font;
class CE_MSDFFont;

class CE_Text
{
public:
	CE_Text(CE_GPUContext& aGPUContext);
	~CE_Text();

	void Init();
	void InitMSDF();

	void Render();

	int GetIndexCount() const { return myIndexCount; }
	const CE_Matrix44f& GetOrientation() const { return myOrientation; }

	void SetColor(const CE_Vector4f& aColor) { myColor = aColor; }
	void SetPosition(const CE_Vector2f& aPosition) { myPosition = aPosition; }
	void SetText(const CE_String& aString);

private:
	void SetupObjectBuffer();
	void SetupVertexAndIndexBuffers(void* aVertexData, void* aIndexData);
	CE_GPUContext& myGPUContext;

	struct VertexType
	{
		CE_Vector3f myPosition;
		CE_Vector2f myUV;
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

	CE_Font* myFont;
	CE_MSDFFont* myMSDFFont;

	bool myGotText;
};