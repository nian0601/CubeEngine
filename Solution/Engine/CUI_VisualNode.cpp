#include "stdafx.h"
#include "CUI_VisualNode.h"
#include "CE_RendererProxy.h"
#include "CUI_Image.h"
#include "CUI_Pin.h"
#include "CUI_Label.h"
#include "CUI_Message.h"


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

	RenderConnections(anRendererProxy);
}

bool CUI_VisualNode::OnMouseMessage(const CUI_MouseMessage& aMessage)
{
	if (aMessage.myType == CUI_MouseMessage::MOUSE_DOWN)
	{
		if (myHasLongPress)
			myPositionOffset = myPosition - aMessage.myNewPosition;

		return true;
	}

	if (aMessage.myType == CUI_MouseMessage::MOUSE_MOVE)
	{
		if (myHasLongPress)
		{
			myPosition = aMessage.myNewPosition+ myPositionOffset;
			return true;
		}

	}

//	return CUI_Container::OnMouseMove(aMessage);
	return false;
}

void CUI_VisualNode::AddPin(bool aIsInput)
{
	CUI_Pin* pin = new CUI_Pin(aIsInput, { myPinSize, myPinSize }, { 0.2f, 0.2f, 0.2f, 1.f });

	AddWidget(pin);

	if (aIsInput)
		myInputs.Add(pin);
	else
		myOutPuts.Add(pin);
}

void CUI_VisualNode::RenderConnections(CE_RendererProxy& anRendererProxy)
{
	int numOutputs = myOutPuts.Size();
	float startOffset = 0.1f;
	float spacing = (1.f - startOffset * 2) / numOutputs;

	for(int i = 0; i < numOutputs; ++i)
	{
		float cutPoint = startOffset + spacing * 0.5f + spacing * i;

		const CUI_Pin* outputPin = myOutPuts[i];
		CE_Vector2f startPosition = outputPin->GetPosition();
		startPosition += outputPin->GetSize() * 0.5f;

		const CE_GrowingArray<CUI_Pin*>& connections = outputPin->GetConnections();
		for(const CUI_Pin* connection : connections)
		{
			CE_Vector2f endPosition = connection->GetPosition();
			endPosition += connection->GetSize() * 0.5f;
		
			RenderSteppedLine(anRendererProxy, startPosition, endPosition, cutPoint);
		}
	}
}

void CUI_VisualNode::RenderSteppedLine(CE_RendererProxy& anRendererProxy, const CE_Vector2f& aStartPos, const CE_Vector2f& aEndPos, float aCutPoint)
{
	float cutPoint = aStartPos.x + (aEndPos.x - aStartPos.x) * aCutPoint;

	CE_Vector2f cutPointA = aStartPos;
	cutPointA.x = cutPoint;

	CE_Vector2f cutPointB = aEndPos;
	cutPointB.x = cutPoint;

	anRendererProxy.Add2DLine(aStartPos, cutPointA);
	anRendererProxy.Add2DLine(cutPointA, cutPointB);
	anRendererProxy.Add2DLine(cutPointB, aEndPos);
}
