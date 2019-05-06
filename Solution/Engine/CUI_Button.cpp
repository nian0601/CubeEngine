#include "stdafx.h"
#include "CUI_Button.h"
#include "CUI_Image.h"
#include "CUI_Label.h"

CUI_Button::CUI_Button(const CE_Vector2f& aSize, const CE_Vector4f& aColor)
{
	myWidgets.Add(new CUI_Image(aSize, aColor));
}

CUI_Button::CUI_Button(const CE_Font& aFont, const CE_String& aText)
{
	myWidgets.Add(new CUI_Label(aFont, aText));
}

bool CUI_Button::OnClick()
{
	myOnClick();
	return true;
}

bool CUI_Button::OnMouseDown(const CUI_MouseMessage& aMessage)
{
	return CUI_Widget::OnMouseDown(aMessage);
}

bool CUI_Button::OnMouseUp(const CUI_MouseMessage& aMessage)
{
	return CUI_Widget::OnMouseUp(aMessage);
}

void CUI_Button::PrepareLayout()
{
	CE_Vector2f position = GetPosition();

	mySize = CE_Vector2f(0.f);
	for (int i = 0; i < myWidgets.Size(); ++i)
	{
		CUI_Widget* child = myWidgets[i];
		child->SetPosition(position);
		child->PrepareLayout();
	
		CE_Vector2f childSize = child->GetSize();
		mySize.x = CE_Max(childSize.x, mySize.x);
		mySize.y = CE_Max(childSize.y, mySize.y);
	}
}
