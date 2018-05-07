#include "stdafx.h"
#include "CUI_Widget.h"
#include "CE_WindowMessage.h"

void CUI_Widget::SetPosition(const CE_Vector2f& aPosition)
{
	myPosition = aPosition;
}

void CUI_Widget::SetSize(const CE_Vector2f& aSize)
{
	mySize = aSize;
}

void CUI_Widget::OnMouseDown(const CE_Vector2f& aMousePosition)
{
	myHasLongPress = false;

	if (!myIsVisible)
		return;

	if (Contains(aMousePosition))
	{
		myHasLongPress = true;
	}
}

bool CUI_Widget::OnMouseUp(const CE_Vector2f& aMousePosition)
{
	if (!myIsVisible)
		return false;

	myIsFocused = false;

	if (!myHasLongPress)
		return false;

	myHasLongPress = false;
	if (Contains(aMousePosition))
	{
		if(CanBeFocused())
			myIsFocused = true;

		return OnClick();
	}

	return false;
}

void CUI_Widget::OnMouseMove(const CE_Vector2f& aNewMousePosition, const CE_Vector2f& aOldMousePosition)
{
	if (!myIsVisible)
		return;

	bool curr = Contains(aNewMousePosition);
	bool old = Contains(aOldMousePosition);

	if (curr && !old)
		OnMouseEnter();
	else if (!curr && old)
		OnMouseExit();
}

bool CUI_Widget::Contains(const CE_Vector2f& aPosition) const
{
	if (myPosition.x > aPosition.x) return false;
	if (myPosition.x + mySize.x < aPosition.x) return false;
	if (myPosition.y > aPosition.y) return false;
	if (myPosition.y + mySize.y < aPosition.y) return false;

	return true;
}

bool CUI_Widget::OnWindowMessage(const CE_WindowMessage& aMessage)
{
	if (aMessage.myType == CE_WindowMessage::CHARACTER)
		return OnTextInput(aMessage);

	return false;
}
