#pragma once

class CE_Camera;
class CE_GPUContext;
class CE_Model;
class CE_CubeShader;

class CE_Sprite;
class CE_SpriteShader;

class CE_Renderer
{
public:
	CE_Renderer(CE_GPUContext& anGPUContext);
	~CE_Renderer();

	void Render(CE_Camera& aCamera);

	void AddCubeData(const CE_Matrix44f& anOrientation, const CE_Vector3f& aScale, const CE_Vector4f& aColor);
	void AddSpriteData(const CE_Vector2f& aPosition, const CE_Vector2f& aSize, const CE_Vector4f& aColor, const CE_Vector2f& aHotspot);

private:
	CE_GPUContext& myGPUContext;

	struct CubeData
	{
		CE_Matrix44f myOrientation;
		CE_Vector4f myColor;
		CE_Vector3f myScale;
	};
	CE_GrowingArray<CubeData> myCubeData;
	CE_CubeShader* myCubeShader;
	CE_Model* myCubeModel;

	struct SpriteData
	{
		CE_Vector4f myColor;
		CE_Vector2f myPosition;
		CE_Vector2f mySize;
		CE_Vector2f myHotspot;
	};
	CE_GrowingArray<SpriteData> mySpriteData;
	CE_Sprite* mySprite;
	CE_SpriteShader* mySpriteShader;
};

