#include "stdafx.h"
#include "CUI_Button.h"

CUI_Button::CUI_Button(const CE_Vector2f& aSize, const CE_Vector4f& aColor)
	: CUI_Image(aSize, aColor)
{
}

bool CUI_Button::OnClick(const CE_Vector2f& aMousePosition)
{
	if (!CUI_Widget::OnClick(aMousePosition))
		return false;

	myOnClick();
	return true;
}
