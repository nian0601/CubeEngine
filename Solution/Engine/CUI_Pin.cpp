#include "stdafx.h"
#include "CUI_Pin.h"
#include "CE_RendererProxy.h"

#include "CE_DebugDraw.h"
#include "CUI_Message.h"

CUI_Pin::CUI_Pin(bool aIsInput, const CE_Vector2f& aSize, const CE_Vector4f& aColor)
	: myColor(aColor)
	, myIsInput(aIsInput)
	, myID(0)
{
	mySize = aSize;
}

void CUI_Pin::Render(CE_RendererProxy& anRendererProxy)
{
	CE_Vector4f color = myColor;
	if (myIsFocused)
		color.x += 0.2f;
	else if (myHasLongPress)
		color.y += 0.1f;
	else if (myIsHovered)
		color.z += 0.1f;

	anRendererProxy.AddSprite(myPosition, mySize, color);
}

bool CUI_Pin::OnDragMessage(CUI_DragMessage& aMessage)
{
	if (aMessage.myType == CUI_DragMessage::DRAG_START)
	{
		return true;
	}

	if (aMessage.myType == CUI_DragMessage::DRAG_END)
	{
		return true;
	}

	return false;
}