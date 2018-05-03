#pragma once


struct CE_ModelData
{
	CE_Matrix44f myOrientation;
	CE_Vector4f myColorAndMetalness;
	CE_Vector4f myScaleAndRoughness;
	bool myIsSphere : 1;
};

struct CE_2DData
{
	enum Type
	{
		TEXT,
		SPRITE,
	};
	Type myType;
	CE_Vector2f myPosition;
	CE_Vector4f myColor;

	// TODO(NA): Can we union these or some other smart thing?
	CE_Vector4f mySizeAndHotspot;
	CE_String myString;
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
	void AddText(const CE_String& aString, const CE_Vector2f& aPosition, const CE_Vector4f& aColor = CE_Vector4f(1.f, 1.f, 1.f, 1.f));
	void AddPointLight(const CE_Matrix44f& anOrientation, const CE_Vector4f& aColorAndIntensity, float aRadius);

	const CE_GrowingArray<CE_ModelData>& GetModelData() const { return myModelData; }
	const CE_GrowingArray<CE_PointLightData>& GetPointLightData() const { return myPointLightData; }
	const CE_GrowingArray<CE_2DData>& Get2DData() const { return my2DData; }
	void Clear();

private:
	CE_GrowingArray<CE_ModelData> myModelData;
	CE_GrowingArray<CE_PointLightData> myPointLightData;
	CE_GrowingArray<CE_2DData> my2DData;
};

