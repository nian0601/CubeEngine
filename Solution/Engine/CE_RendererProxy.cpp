#include "stdafx.h"
#include "CE_RendererProxy.h"
#include "CE_Renderer.h"
#include "CE_Texture.h"

void CE_RendererProxy::AddModel(const CE_Matrix44f& anOrientation, const CE_Vector3f& aScale, const CE_Vector4f& aColor, float aMetalness, float aRoughness, bool aIsSphere)
{
	CE_ModelData& data = myModelData.Add();
	data.myOrientation = anOrientation;
	CE_Vector3f pos = data.myOrientation.GetPos();
	pos.y += aScale.y * 0.5f;
	data.myOrientation.SetPos(pos);

	data.myScaleAndRoughness = CE_Vector4f(aScale.x, aScale.y, aScale.z, aRoughness);
	data.myColorAndMetalness = CE_Vector4f(aColor.x, aColor.y, aColor.z, aMetalness);
	data.myIsSphere = aIsSphere;
}

void CE_RendererProxy::AddObj(const CE_Matrix44f& anOrientation, const CE_Vector3f& aScale, int aObjID)
{
	CE_ObjRenderData& data = myObjRenderData.Add();
	data.myOrientation = anOrientation;
	data.myScale = aScale;
	data.myObjID = aObjID;
}

void CE_RendererProxy::AddSprite(const CE_Vector2f& aPosition, const CE_Vector2f& aSize, const CE_Vector4f& aColor, const CE_Vector2f& aHotspot /*= CE_Vector2f(0.f, 0.f)*/)
{
	CE_2DData& data = my2DData.Add();
	data.myType = CE_2DData::SPRITE;
	data.myPosition = aPosition;
	data.myColor = aColor;
	data.mySizeAndHotspot.x = aSize.x;
	data.mySizeAndHotspot.y = aSize.y;
	data.mySizeAndHotspot.z = aHotspot.x;
	data.mySizeAndHotspot.w = aHotspot.y;
	data.myTexture = nullptr;
}

void CE_RendererProxy::AddSprite(const CE_Vector2f& aPosition, const CE_Texture* aTexture, const CE_Vector2f& aSize, const CE_Vector2f& aHotspot /*= CE_Vector2f(0.f, 0.f)*/)
{
	CE_2DData& data = my2DData.Add();
	data.myType = CE_2DData::SPRITE;
	data.myPosition = aPosition;
	data.myColor = { 1.f, 1.f, 1.f, 1.f };
	data.mySizeAndHotspot.x = aSize.x;
	data.mySizeAndHotspot.y = aSize.y;
	data.mySizeAndHotspot.z = aHotspot.x;
	data.mySizeAndHotspot.w = aHotspot.y;
	data.myTexture = aTexture;
}

void CE_RendererProxy::AddText(const CE_String& aString, const CE_Vector2f& aPosition, const CE_Vector4f& aColor)
{
	CE_2DData& data = my2DData.Add();
	data.myType = CE_2DData::TEXT;
	data.myPosition = aPosition;
	data.myColor = aColor;
	data.myString = aString;
}

void CE_RendererProxy::Add2DLine(const CE_Vector2f& aStart, const CE_Vector2f& aEnd, const CE_Vector4f& aColor)
{
	CE_2DData& data = my2DData.Add();
	data.myType = CE_2DData::LINE;
	data.myPosition = aStart;
	data.mySizeAndHotspot.x = aEnd.x;
	data.mySizeAndHotspot.y = aEnd.y;
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
	myObjRenderData.RemoveAll();
	myPointLightData.RemoveAll();
	my2DData.RemoveAll();
}