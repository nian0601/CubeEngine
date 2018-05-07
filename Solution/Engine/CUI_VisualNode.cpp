#include "stdafx.h"
#include "CUI_VisualNode.h"
#include "CE_RendererProxy.h"
#include "CUI_Image.h"
#include "CUI_Pin.h"


CUI_VisualNode::CUI_VisualNode()
{
	myPosition.x = 256.f;
	myPosition.y = 256.f;

	mySize.x = 128.f;
	mySize.y = 128.f;

	myColor.x = 0.78f;
	myColor.y = 0.88f;
	myColor.z = 0.78f;
	myColor.w = 1.f;

	myPinSize = 16.f;
}

void CUI_VisualNode::PrepareLayout()
{
	CE_Vector2f position = myPosition;

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
	}

	mySize.y = (position.y - myPosition.y) + 10.f;
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
