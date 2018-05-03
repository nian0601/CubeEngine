#include "stdafx.h"
#include "CUI_Image.h"
#include "CE_RendererProxy.h"

CUI_Image::CUI_Image(const CE_Vector4f& aColor)
	: CUI_Image({ 0.f, 0.f }, aColor)
{
}

CUI_Image::CUI_Image(const CE_Vector2f& aSize)
	: CUI_Image(aSize, { 1.f, 1.f, 1.f, 1.f })
{
}

CUI_Image::CUI_Image(const CE_Vector2f& aSize, const CE_Vector4f& aColor)
	: CUI_Image(aSize, aColor, {0.f, 0.f})
{
}

CUI_Image::CUI_Image(const CE_Vector2f& aSize, const CE_Vector4f& aColor, const CE_Vector2f& aHotspot)
	: myColor(aColor)
	, myHotspot(aHotspot)
{
	mySize = aSize;
}

void CUI_Image::Render(CE_RendererProxy& anRendererProxy)
{
	anRendererProxy.AddSprite(myPosition, mySize, myColor, myHotspot);
}