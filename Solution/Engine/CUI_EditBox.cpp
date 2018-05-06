#include "stdafx.h"
#include "CUI_EditBox.h"
#include "CE_Font.h"
#include "CE_RendererProxy.h"
#include "CE_WindowMessage.h"


CUI_EditBox::CUI_EditBox(const CE_Font& aFont, float aWidth)
	: CUI_Label(aFont)
	, myWidth(aWidth)
{
}


CUI_EditBox::~CUI_EditBox()
{
}

void CUI_EditBox::PrepareLayout()
{
	mySize.y = myFont.GetMaxHeight() + 6;
	mySize.x = myWidth;
}

void CUI_EditBox::Render(CE_RendererProxy& anRendererProxy)
{
	CE_Vector2f position = myPosition;
	anRendererProxy.AddSprite(position, mySize, { 0.23f, 0.23f, 0.23f, 1.f });

	position.x += 2.f;
	position.y += 2.f;
	anRendererProxy.AddText(myString, position, { 1.f, 1.f, 1.f, 1.f });
}

bool CUI_EditBox::OnTextInput(const CE_WindowMessage& aMessage)
{
	if (!IsFocused())
		return false;

	switch (aMessage.myWParams)
	{
	case 0x01: //ctrl+a
		break;
	case 0x08:  // backspace 
		myString.RemoveOne();
		break;
	case 0x0A:  // linefeed 
	case 0x1B:  // escape 
	case 0x09:  // tab 
		break;
	case 0x0D:  // carriage return 
		//Finalize editing (Call FocusLost() or similar)
		break;
	default:    // displayable character 
	{
		char character = (char)aMessage.myWParams;
		if (myFont.GetCharData(character))
			myString += character;
	}
		
	}

	return true;
}