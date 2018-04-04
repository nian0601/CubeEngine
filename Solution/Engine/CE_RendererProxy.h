#pragma once


struct CE_CubeData
{
	CE_Matrix44f myOrientation;
	CE_Vector4f myColorAndMetalness;
	CE_Vector4f myScaleAndRoughness;
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

class CE_RendererProxy
{
public:
	void AddCubeData(const CE_Matrix44f& anOrientation, const CE_Vector3f& aScale, const CE_Vector4f& aColor);
	void AddSpriteData(const CE_Vector2f& aPosition, const CE_Vector2f& aSize, const CE_Vector4f& aColor, const CE_Vector2f& aHotspot = CE_Vector2f(0.f, 0.f));
	void AddTextData(const CE_String& aString, const CE_Vector2f& aPosition);

	const CE_GrowingArray<CE_CubeData>& GetCubeData() const { return myCubeData; }
	const CE_GrowingArray<CE_SpriteData>& GetSpriteData() const { return mySpriteData; }
	const CE_GrowingArray<CE_TextData>& GetTextData() const { return myTextData; }
	void Clear();

private:
	CE_GrowingArray<CE_CubeData> myCubeData;
	CE_GrowingArray<CE_SpriteData> mySpriteData;
	CE_GrowingArray<CE_TextData> myTextData;
};

