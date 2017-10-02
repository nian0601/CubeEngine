#include "stdafx.h"
#include "CUI_Label.h"
#include "CE_Font.h"
#include "CE_RendererProxy.h"

CUI_Label::CUI_Label(const CE_Font& aFont)
	: myFont(aFont)
{
}

CUI_Label::CUI_Label(const CE_Font& aFont, const CE_String& aString)
	: CUI_Label(aFont)
{
	SetText(aString);
}

CUI_Label::CUI_Label(const CE_Font& aFont, const char* aString)
	: CUI_Label(aFont)
{
	SetText(aString);
}

void CUI_Label::Render(CE_RendererProxy& anRendererProxy)
{
	anRendererProxy.AddTextData(myString, myPosition);
}

void CUI_Label::SetText(const CE_String& aString)
{
	myString = aString;
	mySize = myFont.GetSize(myString);
}
