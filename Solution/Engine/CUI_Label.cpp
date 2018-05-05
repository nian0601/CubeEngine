#include "stdafx.h"

#include "CE_Font.h"
#include "CE_RendererProxy.h"

#include "CUI_Label.h"
#include "CUI_ValueController.h"

CUI_Label::CUI_Label(const CE_Font& aFont)
	: myFont(aFont)
	, myValueController(nullptr)
	, myColor(1.f, 1.f, 1.f, 1.f)
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

CUI_Label::CUI_Label(const CE_Font& aFont, CUI_ValueController* aValueController)
	: CUI_Label(aFont)
{
	myValueController = aValueController;
}

CUI_Label::~CUI_Label()
{
	CE_SAFE_DELETE(myValueController);
}

void CUI_Label::Render(CE_RendererProxy& anRendererProxy)
{
	CE_Vector4f color = myColor;
	if (myHasLongPress)
		color = CE_Vector4f(0.f, 1.f, 0.f, 1.f);
	else if (myIsHovered)
		color = CE_Vector4f(0.f, 0.f, 1.f, 1.f);

	anRendererProxy.AddText(myString, myPosition, color);
}

void CUI_Label::PrepareLayout()
{
	if (myValueController && myValueController->UpdateValue())
	{
		myString.Clear();
		myValueController->AsString(myString);
	}

	mySize = myFont.GetSize(myString);
}

void CUI_Label::SetText(const CE_String& aString)
{
	myString = aString;
}