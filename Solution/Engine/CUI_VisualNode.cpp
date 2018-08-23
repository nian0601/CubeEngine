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

namespace CUI_VisualNode_priv
{
	const CE_Vector2f locPinSize = CE_Vector2f(16.f, 16.f);
	const CE_Vector4f locDefaultPinColor = CE_Vector4f(0.2f, 0.2f, 0.2f, 1.f);
}

CUI_VisualNode::CUI_VisualNode(const CE_Font& aFont, CN_Node* aRealNode)
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

	myLabel = new CUI_Label(aFont, CN_NodeFactory::GetNodeScreenName(aRealNode->GetIdentifier()));
	myLabel->SetColor({ 0.34f, 0.34f, 0.34f, 1.f });

	for (CN_Pin* realPin : myRealNode->myAllPins)
	{
		CUI_Pin* uiPin = new CUI_Pin(realPin->GetIsInput(), CUI_VisualNode_priv::locPinSize, CE_GetTypeInfo(realPin->GetDataType()).myColor);
		uiPin->myID = realPin->GetPinID();
		uiPin->myNode = this;

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


	float pinSize = 0.f;
	if(myWidgets.Size() > 0)
		pinSize = myWidgets[0]->GetSize().x * 0.5f;

	for (CUI_Widget* widget : myWidgets)
	{
		CUI_Pin* pin = static_cast<CUI_Pin*>(widget);
		if (pin->IsInput())
			position.x = myPosition.x - pinSize;
		else
			position.x = myPosition.x + mySize.x - pinSize;

		position.y += pinSize;

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
