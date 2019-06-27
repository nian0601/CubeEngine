#include "stdafx.h"

#include "CE_RendererProxy.h"

#include "CUI_VisualNode.h"
#include "CUI_Image.h"
#include "CUI_Pin.h"
#include "CUI_Label.h"
#include "CUI_Message.h"

#include "CN_Node.h"
#include "CN_Pin.h"
#include "CN_NodeFactory.h"

CUI_VisualNode::CUI_VisualNode(CN_Node* aRealNode)
	: myID(aRealNode->GetNodeID())
	, myRealNode(aRealNode)
	, myNumOutputPins(0)
{
	myPosition.x = 256.f;
	myPosition.y = 256.f;

	mySize.x = 0.f;
	mySize.y = 0.f;

	myColor.x = 0.78f;
	myColor.y = 0.88f;
	myColor.z = 0.78f;
	myColor.w = 1.f;

	myLabel = new CUI_Label(CN_NodeFactory::GetNodeScreenName(aRealNode->GetIdentifier()));
	myLabel->SetColor({ 0.34f, 0.34f, 0.34f, 1.f });
	myLabel->SetParent(this);

	for (CN_Pin* realPin : myRealNode->myAllPins)
	{
		CUI_Pin* uiPin = new CUI_Pin(*this, realPin);
		AddWidget(uiPin);

		if (!uiPin->IsInput())
			++myNumOutputPins;

		myPins.Add(uiPin);
	}
}

CUI_VisualNode::~CUI_VisualNode()
{
	CE_SAFE_DELETE(myLabel);

	for (CUI_Pin* pin : myPins)
	{
		CE_GrowingArray<CUI_Pin*>& connections = pin->myConnections;
		for (CUI_Pin* connection : connections)
			connection->myConnections.RemoveCyclic(pin);
	}
}

void CUI_VisualNode::PrepareLayout()
{
	CE_Vector2f position = myPosition;

	myLabel->PrepareLayout();
	CE_Vector2f labelSize = myLabel->GetSize();
	mySize.x = labelSize.x;

	position.y += myLabel->GetSize().y;

	// Add some extra spacing below the label
	position.y += 10.f;

	// Should split PrepareLayout into two steps:
	// - PrepareSize
	// - PreparePosition/Layout
	//
	// Sometimes the Position depends on the Size, so we cant set the position
	// of the child before calculating the size, which is done in PrepareLayout,
	// but we also need the position inside PrepareLayout so that we can
	// layout any children of the child.

	for (CUI_Widget* widget : myWidgets)
	{
		widget->PrepareLayout();
		if (mySize.x < widget->GetSize().x)
			mySize.x = widget->GetSize().x;
	}

	
	for (CUI_Widget* widget : myWidgets)
	{
		CUI_Pin* pin = static_cast<CUI_Pin*>(widget);
		if (pin->IsInput())
			position.x = myPosition.x - 8.f;
		else
			position.x = myPosition.x + mySize.x - 8.f;

		widget->SetPosition(position);
		widget->PrepareLayout();

		CE_Vector2f pinSize = widget->GetSize();
		if (pinSize.x > mySize.x)
			mySize.x = pinSize.x;

		// Move down one pin
		position.y += pinSize.y * 0.5f;

		// And add some spacing
		position.y += 10.f;
	}

	mySize.y = (position.y - myPosition.y) + 10.f;

	position.x = myPosition.x + (mySize.x - labelSize.x) * 0.5f;
	position.y = myPosition.y;
	myLabel->SetPosition(position);
}

void CUI_VisualNode::SetPosition(const CE_Vector2f& aPosition)
{
	CUI_Container::SetPosition(aPosition);
	myRealNode->myPosition = aPosition;
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
			myPosition = aMessage.myNewPosition + myPositionOffset;
			myRealNode->myPosition = myPosition + myPositionOffset;
			return true;
		}
	}

	return false;
}

void CUI_VisualNode::ConnectWithNode(CUI_VisualNode* aNode, s32 aPinID, s32 aOtherPinID)
{
	CN_Pin* realOutputPin = myRealNode->GetPin(aPinID);
	CN_Pin* realInputPin = aNode->myRealNode->GetPin(aOtherPinID);

	if (realOutputPin->GetDataType() != realInputPin->GetDataType())
		return;

	if (realInputPin->myConnectedPins.Size() == 0)
	{
		realInputPin->myConnectedPins.Add(realOutputPin);
		realOutputPin->myConnectedPins.Add(realInputPin);
	}

	ConnectWithNodeOnlyVisual(aNode, aPinID, aOtherPinID);
}

void CUI_VisualNode::ConnectWithNodeOnlyVisual(CUI_VisualNode* aNode, s32 aPinID, s32 aOtherPinID)
{
	CUI_Pin* outputPin = GetPin(aPinID);
	CUI_Pin* inputPin = aNode->GetPin(aOtherPinID);

	if (inputPin->myConnections.Size() == 0)
	{
		inputPin->myConnections.Add(outputPin);
		outputPin->myConnections.Add(inputPin);
	}
}

void CUI_VisualNode::DisconnectPin(s32 aPinID)
{
	CUI_Pin* pin = GetPin(aPinID);
	for (CUI_Pin* connectedPin : pin->myConnections)
		connectedPin->myConnections.RemoveCyclic(pin);

	pin->myConnections.RemoveAll();

	// Make sure we update the real node aswell, for realtime-execution updates and such
	CN_Pin* realPin = myRealNode->GetPin(aPinID);
	for (CN_Pin* connectedPin : realPin->myConnectedPins)
		connectedPin->myConnectedPins.RemoveCyclic(realPin);

	realPin->myConnectedPins.RemoveAll();
}

CUI_Pin* CUI_VisualNode::GetPin(u32 aID)
{
	for (CUI_Pin* pin : myPins)
	{
		if (pin->myID == aID)
			return pin;
	}

	return nullptr;
}
