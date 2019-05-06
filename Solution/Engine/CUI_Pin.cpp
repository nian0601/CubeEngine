#include "stdafx.h"
#include "CUI_Pin.h"
#include "CE_RendererProxy.h"

#include "CE_DebugDraw.h"
#include "CUI_Message.h"
#include "CN_Pin.h"
#include "CUI_Label.h"
#include "CUI_Image.h"

namespace CUI_Pin_priv
{
	const float locPinSize = 16.f;
	const float locLabelSpacing = 3.f;
}


CUI_Pin::CUI_Pin(CUI_VisualNode& aNode, const CN_Pin* aRealPin, const CE_Font& aFont)
	: myNode(aNode)
	, myColor(CE_GetTypeInfo(aRealPin->GetDataType()).myColor)
	, myIsInput(aRealPin->GetIsInput())
	, myID(aRealPin->GetPinID())
{
	myLabel = new CUI_Label(aFont, aRealPin->GetName());
	myLabel->SetColor({ 0.f, 0.f, 0.f, 1.f });
	myImage = new CUI_Image({ CUI_Pin_priv::locPinSize, CUI_Pin_priv::locPinSize });
}

void CUI_Pin::PrepareLayout()
{
	myLabel->PrepareLayout();
	myImage->PrepareLayout();

	mySize = myImage->GetSize();

	CE_Vector2f labelSize = myLabel->GetSize();
	mySize.x += labelSize.x + CUI_Pin_priv::locLabelSpacing;

	if (mySize.y < labelSize.y)
		mySize.y = labelSize.y;

	CE_Vector4f color = myColor;
	if (myIsFocused)
		color.x += 0.2f;
	else if (myHasLongPress)
		color.y += 0.1f;
	else if (myIsHovered)
		color.z += 0.1f;

	myImage->SetColor(color);
	myImage->SetPosition(myPosition);

	CE_Vector2f textPos = myPosition;
	if (myIsInput)
		textPos.x += CUI_Pin_priv::locPinSize + CUI_Pin_priv::locLabelSpacing;
	else
		textPos.x -= myLabel->GetSize().x + CUI_Pin_priv::locLabelSpacing;

	myLabel->SetPosition(textPos);
}

void CUI_Pin::Render(CE_RendererProxy& anRendererProxy)
{
	myImage->Render(anRendererProxy);
	myLabel->Render(anRendererProxy);
}

bool CUI_Pin::OnDragMessage(CUI_DragMessage& aMessage)
{
	if (aMessage.myType == CUI_DragMessage::DRAG_START)
	{
		return true;
	}

	if (aMessage.myType == CUI_DragMessage::DRAG_END)
	{
		return true;
	}

	return false;
}

float CUI_Pin::GetPinWidth() const
{
	return CUI_Pin_priv::locPinSize;
}
