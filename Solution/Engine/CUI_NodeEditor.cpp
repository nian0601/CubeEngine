#include "stdafx.h"

#include "CUI_NodeEditor.h"
#include "CUI_VisualNode.h"
#include "CUI_Pin.h"
#include "CUI_Message.h"
#include "CE_RendererProxy.h"
#include "CE_BinaryFileWriter.h"
#include "CE_BinaryFileReader.h"
#include "CUI_Label.h"
#include "CUI_Dropbox.h"

#include "CN_ScriptInitNode.h"
#include "CN_ScriptDrawLineNode.h"
#include "CN_Script3In3OutNode.h"
#include "CN_Pin.h"
#include "CN_NodeFactory.h"
#include "CN_NodeGraph.h"
#include "CUI_HBox.h"
#include "CUI_ValueController.h"
#include "CUI_VBox.h"

CUI_NodeEditor::CUI_NodeEditor()
	: mySelectedNode(nullptr)
	, mySelectedPin(nullptr)
	, myContextualProperties(nullptr)
{
	CN_NodeFactory::RegisterNodes();

	myNextNodeID = 0;

	mySize.x = -1.f;
	mySize.y = -1.f;

	myFilePath = "testgraph.cegraph";

	LoadGraph(myFilePath.c_str());
	
	// Make sure any non-nodewidgets handle OnMouseUp in CUI_NodeEditor::OnMouseUp, 
	// currently we dont rely on CUI_Container::myWidgets to iterate over everything,
	// since the VisualNodes needs specialhandling.
	// Should clean that up at some point.
	myNodeDropbox = new CUI_Dropbox("Nodes");
	myNodeDropbox->Hide();
	myNodeDropbox->myOnSelection = std::bind(&CUI_NodeEditor::OnNodeDropboxSelection, this, std::placeholders::_1, std::placeholders::_2);
	AddWidget(myNodeDropbox);

	CE_GrowingArray<CE_String> nodeNames;
	CN_NodeFactory::GetNodeScreenNames(nodeNames);
	for (const CE_String& nodeName : nodeNames)
		myNodeDropbox->AddLabel(nodeName.c_str());
}

CUI_NodeEditor::~CUI_NodeEditor()
{
}

void CUI_NodeEditor::Render(CE_RendererProxy& anRendererProxy)
{
	CUI_Container::Render(anRendererProxy);

	for (CUI_VisualNode* node : myVisualNodes)
		RenderNodeConnections(anRendererProxy, node);

	if (mySelectedPin)
	{
		CE_Vector2f startPos = mySelectedPin->GetPosition();
		startPos += mySelectedPin->GetPinWidth() * 0.5f;

		RenderSteppedLine(anRendererProxy, startPos, myMousePosition, 0.5f);
	}

	myNodeGraph->Execute();
}

bool CUI_NodeEditor::OnMouseUp(const CUI_MouseMessage& aMessage)
{
	SelectNode(nullptr);

	for (CUI_VisualNode* node : myVisualNodes)
	{
		bool handled = node->OnMouseUp(aMessage);

		if (node->IsFocused())
			SelectNode(node);

		if (handled)
			return true;
	}

	if (myNodeDropbox->OnMouseUp(aMessage))
		return true;

	if (myContextualProperties && myContextualProperties->OnMouseUp(aMessage))
		return true;

	return CUI_Widget::OnMouseUp(aMessage);
}

bool CUI_NodeEditor::OnMouseMessage(const CUI_MouseMessage& aMessage)
{
	if (aMessage.myType == CUI_MouseMessage::MOUSE_MOVE)
	{
		myMousePosition = aMessage.myNewPosition;
	}

	if (aMessage.myType == CUI_MouseMessage::MOUSE_DOWN)
	{
		if (aMessage.myMouseButton == CUI_MouseMessage::MOUSE_LEFT)
		{
			if (myNodeDropbox->IsVisible())
			{
				myNodeDropbox->Hide();
				return true;
			}
		}
		else if (aMessage.myMouseButton == CUI_MouseMessage::MOUSE_RIGHT)
		{
			myNodeDropbox->SetPosition(aMessage.myNewPosition);
			myNodeDropbox->Show();
			myNodeDropbox->SetExpansion(true);
			return true;
		}
	}

	return false;
}

bool CUI_NodeEditor::OnDragBegin(CUI_DragMessage& aMessage)
{
	mySelectedPin = nullptr;

	for (CUI_VisualNode* node : myVisualNodes)
	{
		CE_GrowingArray<CUI_Pin*>& pins = node->myPins;
		for (CUI_Pin* pin : pins)
		{
			if (OnPinDragBegin(aMessage, pin))
				return true;
		}
	}

	return false;
}

bool CUI_NodeEditor::OnDragEnd(CUI_DragMessage& aMessage)
{
	CUI_Pin* pin1 = mySelectedPin;
	mySelectedPin = nullptr;

	if (!pin1)
		return false;

	CUI_Pin* pin2 = GetDragEndPin(aMessage);
	if (!pin2)
		return false;

	bool firstPinIsOutput = !pin1->IsInput();
	bool secondPinIsOutput = !pin2->IsInput();
	//We cant connect input to input, or output to output
	if (firstPinIsOutput == secondPinIsOutput)
		return false;

	if (firstPinIsOutput)
		ConnectPins(pin1, pin2);
	else
		ConnectPins(pin2, pin1);

	return true;
}

bool CUI_NodeEditor::OnPinDragBegin(CUI_DragMessage& aMessage, CUI_Pin* aPin)
{
	if (!aPin->OnDragBegin(aMessage))
		return false;

	if (aPin->IsInput())
	{
		CE_GrowingArray<CUI_Pin*>& connections = aPin->myConnections;
		if (connections.Size() > 0)
		{
			mySelectedPin = connections[0];
			aPin->myNode.DisconnectPin(aPin->myID);
		}
		else
		{
			mySelectedPin = aPin;
		}

		aMessage.myData.Set<CUI_Pin*>(mySelectedPin);
	}
	else
	{
		mySelectedPin = aPin;
		aMessage.myData.Set<CUI_Pin*>(mySelectedPin);
	}

	return true;
}

void CUI_NodeEditor::SaveGraph()
{
	myNodeGraph->Save(myFilePath.c_str());
}

void CUI_NodeEditor::DeleteSelectedNode()
{
	if (mySelectedNode)
	{
		myNodeGraph->DeleteNode(mySelectedNode->myRealNode);
		myVisualNodes.RemoveCyclic(mySelectedNode);

		CUI_VisualNode* node = mySelectedNode;
		SelectNode(nullptr);
		DeleteWidget(node);
	}
}

void CUI_NodeEditor::RenderNodeConnections(CE_RendererProxy& anRendererProxy, CUI_VisualNode* aNode)
{
	int numOutputs = aNode->myNumOutputPins;
	float startOffset = 0.1f;
	float spacing = (1.f - startOffset * 2) / numOutputs;

	int count = 0;
	for (const CUI_Pin* pin : aNode->myPins)
	{
		if(pin->IsInput())
			continue;

		float cutPoint = startOffset + spacing * 0.5f + spacing * count;

		CE_Vector2f startPosition = pin->GetPosition();
		startPosition += pin->GetPinWidth() * 0.5f;

		const CE_GrowingArray<CUI_Pin*>& connections = pin->GetConnections();
		for (const CUI_Pin* connection : connections)
		{
			CE_Vector2f endPosition = connection->GetPosition();
			endPosition += connection->GetPinWidth() * 0.5f;

			RenderSteppedLine(anRendererProxy, startPosition, endPosition, cutPoint, pin->myColor);
		}

		count++;
	}
}

void CUI_NodeEditor::RenderSteppedLine(CE_RendererProxy& anRendererProxy, const CE_Vector2f& aStartPos, const CE_Vector2f& aEndPos, float aCutPoint)
{
	RenderSteppedLine(anRendererProxy, aStartPos, aEndPos, aCutPoint, CE_Vector4f(1.f, 1.f, 1.f, 1.f));
}

void CUI_NodeEditor::RenderSteppedLine(CE_RendererProxy& anRendererProxy, const CE_Vector2f& aStartPos, const CE_Vector2f& aEndPos, float aCutPoint, const CE_Vector4f& aColor)
{
	float cutPoint = aStartPos.x + (aEndPos.x - aStartPos.x) * aCutPoint;

	CE_Vector2f cutPointA = aStartPos;
	cutPointA.x = cutPoint;

	CE_Vector2f cutPointB = aEndPos;
	cutPointB.x = cutPoint;

	anRendererProxy.Add2DLine(aStartPos, cutPointA, aColor);
	anRendererProxy.Add2DLine(cutPointA, cutPointB, aColor);
	anRendererProxy.Add2DLine(cutPointB, aEndPos, aColor);
}

CUI_Pin* CUI_NodeEditor::GetDragEndPin(CUI_DragMessage& aMessage)
{
	for (CUI_VisualNode* node : myVisualNodes)
	{
		CE_GrowingArray<CUI_Pin*>& pins = node->myPins;
		for (CUI_Pin* pin : pins)
		{
			if (pin->OnDragEnd(aMessage))
				return pin;
		}
	}

	return nullptr;
}

CUI_VisualNode* CUI_NodeEditor::CreateVisualNode(CN_Node* aRealNode)
{
	CUI_VisualNode* visualNode = new CUI_VisualNode(aRealNode);

	myVisualNodes.Add(visualNode);
	AddWidget(visualNode);

	return visualNode;
}

void CUI_NodeEditor::ConnectPins(u32 aOutputNode, u32 aOutputPin, u32 aInputNode, u32 aInputPin)
{
	CUI_VisualNode* outputNode = nullptr;
	CUI_VisualNode* inputNode = nullptr;

	for (CUI_VisualNode* node : myVisualNodes)
	{
		if (node->myID == aOutputNode)
			outputNode = node;
		else if (node->myID == aInputNode)
			inputNode = node;
	}

	outputNode->ConnectWithNode(inputNode, aOutputPin, aInputPin);
}

void CUI_NodeEditor::ConnectPins(CUI_Pin* aOutputPin, CUI_Pin* aInputPin)
{
	CUI_VisualNode& outputNode = aOutputPin->myNode;
	CUI_VisualNode& inputNode = aInputPin->myNode;

	outputNode.ConnectWithNode(&inputNode, aOutputPin->myID, aInputPin->myID);
}

void CUI_NodeEditor::ConnectVisualPins(u32 aOutputNode, u32 aOutputPin, u32 aInputNode, u32 aInputPin)
{
	CUI_VisualNode* outputNode = nullptr;
	CUI_VisualNode* inputNode = nullptr;

	for (CUI_VisualNode* node : myVisualNodes)
	{
		if (node->myID == aOutputNode)
			outputNode = node;
		else if (node->myID == aInputNode)
			inputNode = node;
	}

	outputNode->ConnectWithNodeOnlyVisual(inputNode, aOutputPin, aInputPin);
}

void CUI_NodeEditor::LoadGraph(const char* aFilePath)
{
	myNodeGraph = new CN_NodeGraph();
	myNodeGraph->Load(aFilePath);
	myNextNodeID = myNodeGraph->myMaxNodeID;

	CreateVisualNodesFromNodeGraph();
}

void CUI_NodeEditor::CreateVisualNodesFromNodeGraph()
{
	for (CN_Node* realNode : myNodeGraph->myNodes)
	{
		CUI_VisualNode* visualNode = CreateVisualNode(realNode);
		visualNode->SetPosition(realNode->myPosition);
	}

	for (CN_Node* realNode : myNodeGraph->myNodes)
	{
		u32 outputNodeID = realNode->GetNodeID();
		for (CN_Pin* pin : realNode->myAllPins)
		{
			if (pin->GetIsInput())
				continue;

			u32 outputPinID = pin->GetPinID();

			for (CN_Pin* connectedPin : pin->GetConnectedPins())
			{
				u32 inputNodeID = connectedPin->GetNode()->GetNodeID();
				u32 inputPInID = connectedPin->GetPinID();

				ConnectVisualPins(outputNodeID, outputPinID, inputNodeID, inputPInID);
			}
		}
	}
}

void CUI_NodeEditor::OnNodeDropboxSelection(CUI_Widget* aWidget, int /*aWidgetIndex*/)
{
	myNodeDropbox->SetExpansion(false);
	myNodeDropbox->Hide();

	CUI_Label* label = static_cast<CUI_Label*>(aWidget);
	const CE_String& text = label->GetText();

	CN_Node* realNode = CN_NodeFactory::CreateNodeFromScreenName(text.c_str());
	realNode->myNodeID = myNextNodeID++;
	myNodeGraph->AddNode(realNode);

	CUI_VisualNode* visualNode = CreateVisualNode(realNode);
	visualNode->SetPosition(myMousePosition);
}

void CUI_NodeEditor::SelectNode(CUI_VisualNode* aNode)
{
	if (mySelectedNode != aNode && myContextualProperties)
	{
		DeleteWidget(myContextualProperties);
		myContextualProperties = nullptr;
	}

	mySelectedNode = aNode;
	if (mySelectedNode)
	{
		myContextualProperties = new CUI_VBox();

		for (CUI_Pin* pin : mySelectedNode->myPins)
		{
			if (!pin->IsInput())
				continue;

			if (pin->IsConnected())
				continue;

			unsigned int pinDataType = pin->GetDataType();

			CE_Any& pinData = pin->myRealPin->myData;
			if (pinDataType == CE_GetTypeID<CE_Vector2f>())
			{
				CE_Vector2f& vector = pinData.Get<CE_Vector2f>();
				myContextualProperties->AddWidget(CreateVectorWidget(pin->myLabel->GetText().c_str(), vector));
			}
			else if (pinDataType == CE_GetTypeID<CE_Vector3f>())
			{
				CE_Vector3f& vector = pinData.Get<CE_Vector3f>();
				myContextualProperties->AddWidget(CreateVectorWidget(pin->myLabel->GetText().c_str(), vector));
			}
			else if (pinDataType == CE_GetTypeID<CE_Vector4f>())
			{
				CE_Vector4f& vector = pinData.Get<CE_Vector4f>();
				myContextualProperties->AddWidget(CreateVectorWidget(pin->myLabel->GetText().c_str(), vector));
			}
		}

		AddWidget(myContextualProperties);
	}

	
}

CUI_HBox* CUI_NodeEditor::CreateVectorWidget(const char* aText, CE_Vector2f& aVector)
{
	CUI_HBox* xBox = CreateFloatController("X: ", aVector.x);
	CUI_HBox* yBox = CreateFloatController("Y: ", aVector.y);

	CUI_HBox* widget = new CUI_HBox();
	widget->AddWidget(new CUI_Label(aText));
	widget->AddWidget(xBox);
	widget->AddWidget(yBox);
	return widget;
}

CUI_HBox* CUI_NodeEditor::CreateVectorWidget(const char* aText, CE_Vector3f& aVector)
{
	CUI_HBox* xBox = CreateFloatController("X: ", aVector.x);
	CUI_HBox* yBox = CreateFloatController("Y: ", aVector.y);
	CUI_HBox* zBox = CreateFloatController("Z: ", aVector.z);

	CUI_HBox* widget = new CUI_HBox();
	widget->AddWidget(new CUI_Label(aText));
	widget->AddWidget(xBox);
	widget->AddWidget(yBox);
	widget->AddWidget(zBox);
	return widget;
}

CUI_HBox* CUI_NodeEditor::CreateVectorWidget(const char* aText, CE_Vector4f& aVector)
{
	CUI_HBox* xBox = CreateFloatController("X: ", aVector.x);
	CUI_HBox* yBox = CreateFloatController("Y: ", aVector.y);
	CUI_HBox* zBox = CreateFloatController("Z: ", aVector.z);
	CUI_HBox* wBox = CreateFloatController("W: ", aVector.w);

	CUI_HBox* widget = new CUI_HBox();
	widget->AddWidget(new CUI_Label(aText));
	widget->AddWidget(xBox);
	widget->AddWidget(yBox);
	widget->AddWidget(zBox);
	widget->AddWidget(wBox);
	return widget;
}

CUI_HBox* CUI_NodeEditor::CreateFloatController(const char* aText, float& aValue)
{
	CUI_HBox* box = new CUI_HBox();
	box->AddWidget(new CUI_Label(aText));

	CUI_ValueController* controller = new CUI_ValueController(&aValue);
	box->AddWidget(new CUI_Label(controller));

	return box;
}