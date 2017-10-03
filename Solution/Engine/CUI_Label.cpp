#include "stdafx.h"

#include "CE_Font.h"
#include "CE_RendererProxy.h"

#include "CUI_Label.h"
#include "CUI_ValueController.h"

CUI_Label::CUI_Label(const CE_Font& aFont)
	: myFont(aFont)
	, myValueController(nullptr)
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
	anRendererProxy.AddTextData(myString, myPosition);
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
