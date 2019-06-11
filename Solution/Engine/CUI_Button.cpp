#include "stdafx.h"
#include "CUI_Button.h"
#include "CUI_Image.h"
#include "CUI_Label.h"
#include "CUI_Manager.h"

CUI_Button::CUI_Button(const CE_String& aText)
{
	myNormalTexture = GetUIManager().GetTexture("button02");
	myHoverTexture = GetUIManager().GetTexture("button04");
	myPressTexture = GetUIManager().GetTexture("button03");

	myImage = new CUI_Image(myNormalTexture);
	myImage->SetParent(this);
	myLabel = new CUI_Label(aText);
	myLabel->SetParent(this);
}

void CUI_Button::PrepareLayout()
{
	if (myHasLongPress)
		myImage->SetTexture(myPressTexture);
	else if (myIsHovered)
		myImage->SetTexture(myHoverTexture);
	else
		myImage->SetTexture(myNormalTexture);

	myImage->PrepareLayout();
	myLabel->PrepareLayout();

	mySize = myImage->GetSize();

	CE_Vector2f position = GetPosition();

	myImage->SetPosition(position);

	//CE_Vector2f sizeDiff = mySize - myLabel->GetSize();
	//sizeDiff *= 0.5f;
	//position.x += sizeDiff.x;
	//position.y += sizeDiff.y;
	myLabel->SetPosition(position);

}

void CUI_Button::Render(CE_RendererProxy& anRendererProxy)
{
	myImage->Render(anRendererProxy);
	myLabel->Render(anRendererProxy);
}


bool CUI_Button::OnClick()
{
	myOnClick();
	return true;
}