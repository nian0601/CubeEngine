#include "stdafx.h"
#include "CE_RendererProxy.h"
#include "CE_Renderer.h"


CE_RendererProxy::CE_RendererProxy(CE_Renderer& aRenderer)
	: myRenderer(aRenderer)
{
}

void CE_RendererProxy::AddCubeData(const CE_Matrix44f& anOrientation, const CE_Vector3f& aScale, const CE_Vector4f& aColor)
{
	myRenderer.AddCubeData(anOrientation, aScale, aColor);
}

void CE_RendererProxy::AddSpriteData(const CE_Vector2f& aPosition, const CE_Vector2f& aSize, const CE_Vector4f& aColor, const CE_Vector2f& aHotspot /*= CE_Vector2f(0.f, 0.f)*/)
{
	myRenderer.AddSpriteData(aPosition, aSize, aColor, aHotspot);
}

void CE_RendererProxy::AddTextData(const CE_String& aString, const CE_Vector2f& aPosition)
{
	myRenderer.AddTextData(aString, aPosition);
}
