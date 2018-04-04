#include "stdafx.h"
#include "CE_RendererProxy.h"
#include "CE_Renderer.h"

void CE_RendererProxy::AddCubeData(const CE_Matrix44f& anOrientation, const CE_Vector3f& aScale, const CE_Vector4f& aColor)
{
	CE_CubeData& data = myCubeData.Add();
	data.myOrientation = anOrientation;
	data.myScaleAndRoughness = CE_Vector4f(aScale.x, aScale.y, aScale.z, 0.f);
	data.myColorAndMetalness = aColor;
}

void CE_RendererProxy::AddSpriteData(const CE_Vector2f& aPosition, const CE_Vector2f& aSize, const CE_Vector4f& aColor, const CE_Vector2f& aHotspot /*= CE_Vector2f(0.f, 0.f)*/)
{
	CE_SpriteData& data = mySpriteData.Add();
	data.myPosition = aPosition;
	data.mySize = aSize;
	data.myColor = aColor;
	data.myHotspot = aHotspot;
}

void CE_RendererProxy::AddTextData(const CE_String& aString, const CE_Vector2f& aPosition)
{
	CE_TextData& data = myTextData.Add();
	data.myString = aString;
	data.myPosition = aPosition;
}

void CE_RendererProxy::Clear()
{
	myCubeData.RemoveAll();
	mySpriteData.RemoveAll();
	myTextData.RemoveAll();
}
