#include "stdafx.h"

#include "CE_Font.h"
#include "CE_RendererProxy.h"

#include "CUI_Label.h"
#include "CUI_ValueController.h"
#include "CUI_Message.h"
#include "CUI_Manager.h"

CUI_Label::CUI_Label()
	: myValueController(nullptr)
	, myColor(1.f, 1.f, 1.f, 1.f)
{
}

CUI_Label::CUI_Label(const CE_String& aString)
	: CUI_Label()
{
	SetText(aString);
}

CUI_Label::CUI_Label(const char* aString)
	: CUI_Label()
{
	SetText(aString);
}

CUI_Label::CUI_Label(CUI_ValueController* aValueController)
	: CUI_Label()
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

	const CUI_Manager& manager = GetUIManager();
	mySize = manager.GetFont().GetSize(myString);
}

bool CUI_Label::OnMouseMessage(const CUI_MouseMessage& aMessage)
{
	if (aMessage.myType != CUI_MouseMessage::MOUSE_WHEEL || !myValueController)
		return false;

	myValueController->OnScroll(aMessage);
	return true;
}

void CUI_Label::SetText(const CE_String& aString)
{
	myString = aString;
}