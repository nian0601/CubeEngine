#pragma once


struct CE_ModelData
{
	CE_Matrix44f myOrientation;
	CE_Vector4f myColorAndMetalness;
	CE_Vector4f myScaleAndRoughness;
	bool myIsSphere : 1;
};

struct CE_SpriteData
{
	CE_Vector4f myColor;
	CE_Vector2f myPosition;
	CE_Vector2f mySize;
	CE_Vector2f myHotspot;
};

struct CE_TextData
{
	CE_String myString;
	CE_Vector2f myPosition;
};

struct CE_PointLightData
{
	CE_Matrix44f myOrientation;
	CE_Vector4f myColorAndIntensity;
	float myRadius;
};

class CE_RendererProxy
{
public:
	void AddModel(const CE_Matrix44f& anOrientation, const CE_Vector3f& aScale, const CE_Vector4f& aColor, float aMetalness, float aRoughness, bool aIsSphere = false);
	void AddSprite(const CE_Vector2f& aPosition, const CE_Vector2f& aSize, const CE_Vector4f& aColor, const CE_Vector2f& aHotspot = CE_Vector2f(0.f, 0.f));
	void AddText(const CE_String& aString, const CE_Vector2f& aPosition);
	void AddPointLight(const CE_Matrix44f& anOrientation, const CE_Vector4f& aColorAndIntensity, float aRadius);

	const CE_GrowingArray<CE_ModelData>& GetModelData() const { return myModelData; }
	const CE_GrowingArray<CE_SpriteData>& GetSpriteData() const { return mySpriteData; }
	const CE_GrowingArray<CE_TextData>& GetTextData() const { return myTextData; }
	const CE_GrowingArray<CE_PointLightData>& GetPointLightData() const { return myPointLightData; }
	void Clear();

private:
	CE_GrowingArray<CE_ModelData> myModelData;
	CE_GrowingArray<CE_SpriteData> mySpriteData;
	CE_GrowingArray<CE_TextData> myTextData;
	CE_GrowingArray<CE_PointLightData> myPointLightData;
};

