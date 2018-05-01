#include "stdafx.h"
#include "CE_RendererProxy.h"
#include "CE_Renderer.h"

void CE_RendererProxy::AddModel(const CE_Matrix44f& anOrientation, const CE_Vector3f& aScale, const CE_Vector4f& aColor, float aMetalness, float aRoughness, bool aIsSphere)
{
	CE_ModelData& data = myModelData.Add();
	data.myOrientation = anOrientation;
	data.myScaleAndRoughness = CE_Vector4f(aScale.x, aScale.y, aScale.z, aRoughness);
	data.myColorAndMetalness = CE_Vector4f(aColor.x, aColor.y, aColor.z, aMetalness);
	data.myIsSphere = aIsSphere;
}

void CE_RendererProxy::AddSprite(const CE_Vector2f& aPosition, const CE_Vector2f& aSize, const CE_Vector4f& aColor, const CE_Vector2f& aHotspot /*= CE_Vector2f(0.f, 0.f)*/)
{
	CE_SpriteData& data = mySpriteData.Add();
	data.myPosition = aPosition;
	data.mySize = aSize;
	data.myColor = aColor;
	data.myHotspot = aHotspot;
}

void CE_RendererProxy::AddText(const CE_String& aString, const CE_Vector2f& aPosition, const CE_Vector4f& aColor)
{
	CE_TextData& data = myTextData.Add();
	data.myString = aString;
	data.myPosition = aPosition;
	data.myColor = aColor;
}

void CE_RendererProxy::AddPointLight(const CE_Matrix44f& anOrientation, const CE_Vector4f& aColorAndIntensity, float aRadius)
{
	CE_PointLightData& data = myPointLightData.Add();
	data.myOrientation = anOrientation;
	data.myColorAndIntensity = aColorAndIntensity;
	data.myRadius = aRadius;
}

void CE_RendererProxy::Clear()
{
	myModelData.RemoveAll();
	mySpriteData.RemoveAll();
	myTextData.RemoveAll();
	myPointLightData.RemoveAll();
}
