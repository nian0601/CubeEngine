#include "stdafx.h"
#include "CUI_Widget.h"
#include "CE_WindowMessage.h"
#include "CUI_Message.h"

void CUI_Widget::SetPosition(const CE_Vector2f& aPosition)
{
	myPosition = aPosition;
}

void CUI_Widget::SetSize(const CE_Vector2f& aSize)
{
	mySize = aSize;
}

bool CUI_Widget::OnMouseDown(const CUI_MouseMessage& aMessage)
{
	myHasLongPress = false;

	if (!myIsVisible)
		return false;

	if (Contains(aMessage.myNewPosition))
	{
		myHasLongPress = true;

		OnMouseMessage(aMessage);

		return true;
	}

	return false;
}

bool CUI_Widget::OnMouseUp(const CUI_MouseMessage& aMessage)
{
	if (!myIsVisible)
		return false;

	myIsFocused = false;

	if (!myHasLongPress)
		return false;

	myHasLongPress = false;
	if (Contains(aMessage.myNewPosition))
	{
		if(CanBeFocused())
			myIsFocused = true;

		OnMouseMessage(aMessage);

		return OnClick();
	}

	return false;
}

bool CUI_Widget::OnMouseMove(const CUI_MouseMessage& aMessage)
{
	if (!myIsVisible)
		return false;

	bool curr = Contains(aMessage.myNewPosition);
	bool old = Contains(aMessage.myOldPosition);

	if (curr)
	{
		if (!old)
			OnMouseEnter();

		OnMouseMessage(aMessage);
		return true;
	}

	if (old)
		OnMouseExit();

	return false;
}

bool CUI_Widget::OnDragBegin(CUI_DragMessage& aMessage)
{
	if (!myIsVisible)
		return false;

	if (!myHasLongPress)
		return false;

	return OnDragMessage(aMessage);
}

bool CUI_Widget::OnDragEnd(CUI_DragMessage& aMessage)
{
	if (!myIsVisible)
		return false;

	if (!myIsHovered)
		return false;

	return OnDragMessage(aMessage);
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