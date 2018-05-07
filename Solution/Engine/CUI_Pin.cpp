#include "stdafx.h"
#include "CUI_Pin.h"
#include "CE_RendererProxy.h"

#include "CE_DebugDraw.h"

CUI_Pin::CUI_Pin(bool aIsInput, const CE_Vector2f& aSize, const CE_Vector4f& aColor)
	: myColor(aColor)
	, myIsInput(aIsInput)
{
	mySize = aSize;
}

void CUI_Pin::Render(CE_RendererProxy& anRendererProxy)
{
	anRendererProxy.AddSprite(myPosition, mySize, myColor);
}

void CUI_Pin::OnMouseMove(const CE_Vector2f& aNewMousePosition, const CE_Vector2f& /*aOldMousePosition*/)
{
	aNewMousePosition;
	//CE_DRAW_LINE(myPosition, aNewMousePosition);
}
