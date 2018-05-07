#include "stdafx.h"
#include "CUI_VisualNode.h"
#include "CE_RendererProxy.h"
#include "CUI_Image.h"
#include "CUI_Pin.h"
#include "CUI_Label.h"


CUI_VisualNode::CUI_VisualNode(const CE_Font& aFont, const char* aLabel)
{
	myPosition.x = 256.f;
	myPosition.y = 256.f;

	mySize.x = 0.f;
	mySize.y = 0.f;

	myColor.x = 0.78f;
	myColor.y = 0.88f;
	myColor.z = 0.78f;
	myColor.w = 1.f;

	myPinSize = 16.f;

	myLabel = new CUI_Label(aFont, aLabel);
	myLabel->SetColor({ 0.34f, 0.34f, 0.34f, 1.f });
}

CUI_VisualNode::~CUI_VisualNode()
{
	CE_SAFE_DELETE(myLabel);
}

void CUI_VisualNode::PrepareLayout()
{
	CE_Vector2f position = myPosition;

	myLabel->PrepareLayout();
	CE_Vector2f labelSize = myLabel->GetSize();
	mySize.x = labelSize.x;

	position.y += myLabel->GetSize().y;

	for (CUI_Widget* widget : myWidgets)
	{
		CUI_Pin* pin = static_cast<CUI_Pin*>(widget);
		if(pin->IsInput())
			position.x = myPosition.x - myPinSize*0.5f;
		else
			position.x = myPosition.x + mySize.x - myPinSize*0.5f;

		position.y += myPinSize*0.5f;

		widget->SetPosition(position);
		widget->PrepareLayout();

		position.y += 10.f;

		if (widget->GetSize().x > mySize.x)
			mySize.x = widget->GetSize().x;
	}

	mySize.y = (position.y - myPosition.y) + 10.f;

	position.x = myPosition.x + (mySize.x - labelSize.x) * 0.5f;
	position.y = myPosition.y;
	myLabel->SetPosition(position);
}

void CUI_VisualNode::Render(CE_RendererProxy& anRendererProxy)
{
	CE_Vector4f color = myColor;
	if (myIsFocused)
		color.x += 0.2f;
	else if (myHasLongPress)
		color.y += 0.1f;
	else if (myIsHovered)
		color.z += 0.1f;

	anRendererProxy.AddSprite(myPosition, mySize, color);
	myLabel->Render(anRendererProxy);
	CUI_Container::Render(anRendererProxy);
}

void CUI_VisualNode::OnMouseDown(const CE_Vector2f& aMousePosition)
{
	CUI_Widget::OnMouseDown(aMousePosition);

	if (myHasLongPress)
	{
		myPositionOffset = myPosition - aMousePosition;
	}
}

void CUI_VisualNode::OnMouseMove(const CE_Vector2f& aNewMousePosition, const CE_Vector2f& aOldMousePosition)
{
	CUI_Widget::OnMouseMove(aNewMousePosition, aOldMousePosition);
	
	if (myHasLongPress)
		myPosition = aNewMousePosition + myPositionOffset;
}

void CUI_VisualNode::AddPin(bool aIsInput)
{
	CUI_Pin* pin = new CUI_Pin(aIsInput, { myPinSize, myPinSize }, { 0.2f, 0.9f, 0.12f, 1.f });

	AddWidget(pin);
}
