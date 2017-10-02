#include "stdafx.h"
#include "CUI_Widget.h"

void CUI_Widget::SetPosition(const CE_Vector2f& aPosition)
{
	myPosition = aPosition;
}

void CUI_Widget::SetSize(const CE_Vector2f& aSize)
{
	mySize = aSize;
}

bool CUI_Widget::OnClick(const CE_Vector2f& aMousePosition)
{
	if (myPosition.x > aMousePosition.x) return false;
	if (myPosition.x + mySize.x < aMousePosition.x) return false;
	if (myPosition.y > aMousePosition.y) return false;
	if (myPosition.y + mySize.y < aMousePosition.y) return false;

	return true;
}