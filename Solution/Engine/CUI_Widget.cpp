#include "stdafx.h"
#include "CUI_Widget.h"

void CUI_Widget::FinalizeLayout()
{
	UpdateScreenPosition();
	UpdateScreenSize();
}

void CUI_Widget::SetPosition(const CE_Vector2f& aPosition)
{
	myPosition = aPosition;
	UpdateScreenPosition();
}

void CUI_Widget::SetSize(const CE_Vector2f& aSize)
{
	mySize = aSize;
	UpdateScreenSize();
}

void CUI_Widget::SetWindowSize(const CE_Vector2f& aSize)
{
	myWindowSize = aSize;
}

bool CUI_Widget::OnClick(const CE_Vector2f& aMousePosition)
{
	if (myScreenPosition.x > aMousePosition.x) return false;
	if (myScreenPosition.x + myScreenSize.x < aMousePosition.x) return false;
	if (myScreenPosition.y > aMousePosition.y) return false;
	if (myScreenPosition.y + myScreenSize.y < aMousePosition.y) return false;

	return true;
}

void CUI_Widget::UpdateScreenPosition()
{
	myScreenPosition = myPosition * myWindowSize;
}

void CUI_Widget::UpdateScreenSize()
{
	myScreenSize = mySize * myWindowSize;
}