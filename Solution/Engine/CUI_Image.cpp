#include "stdafx.h"
#include "CUI_Image.h"
#include "CE_RendererProxy.h"
#include "CUI_Manager.h"
#include "CE_Texture.h"

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
	, myTexture(nullptr)
{
	mySize = aSize;
}

CUI_Image::CUI_Image(const char* aTexture)
	: myColor(1.f, 1.f, 1.f, 1.f)
	, myHotspot(0.f, 0.f)
{
	myTexture = GetUIManager().GetTexture(aTexture);
	mySize.x = float(myTexture->GetSize().x);
	mySize.y = float(myTexture->GetSize().y);
}

CUI_Image::CUI_Image(const CE_Texture* aTexture)
	: myColor(1.f, 1.f, 1.f, 1.f)
	, myHotspot(0.f, 0.f)
	, myTexture(aTexture)
{
	mySize.x = float(myTexture->GetSize().x);
	mySize.y = float(myTexture->GetSize().y);
}

void CUI_Image::Render(CE_RendererProxy& anRendererProxy)
{
	if(myTexture)
		anRendererProxy.AddSprite(myPosition, myTexture, mySize);
	else
		anRendererProxy.AddSprite(myPosition, mySize, myColor, myHotspot);
}