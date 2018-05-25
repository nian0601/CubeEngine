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
#include "CN_Pin.h"

namespace CUI_NodeEditor_priv
{
	const CE_Vector2f locPinSize = CE_Vector2f(16.f, 16.f);
	const CE_Vector4f locDefaultPinColor = CE_Vector4f(0.2f, 0.2f, 0.2f, 1.f);
}

CUI_NodeEditor::CUI_NodeEditor(CE_GPUContext& aGPUContext)
{
	myNextNodeID = 0;

	mySize.x = -1.f;
	mySize.y = -1.f;

	myFont = new CE_Font();
	myFont->LoadFromFile("Data/Font/Decent_Font.png", aGPUContext);

	myFilePath = "testgraph.cegraph";

	LoadGraph(myFilePath.c_str());
	
	myNodeDropbox = new CUI_Dropbox(*myFont, "Nodes");
	myNodeDropbox->Hide();
	myNodeDropbox->myOnSelection = std::bind(&CUI_NodeEditor::OnNodeDropboxSelection, this, std::placeholders::_1);
	AddWidget(myNodeDropbox);

	myNodeDropbox->AddLabel("Init");
	myNodeDropbox->AddLabel("Line Node");
}

CUI_NodeEditor::~CUI_NodeEditor()
{
	CE_SAFE_DELETE(myFont);
}

void CUI_NodeEditor::Render(CE_RendererProxy& anRendererProxy)
{
	CUI_Container::Render(anRendererProxy);

	for (CUI_VisualNode* node : myVisualNodes)
		RenderNodeConnections(anRendererProxy, node);

	if (mySelectedPin)
	{
		CE_Vector2f startPos = mySelectedPin->GetPosition();
		startPos += CUI_NodeEditor_priv::locPinSize * 0.5f;

		RenderSteppedLine(anRendererProxy, startPos, myMousePosition, 0.5f);
	}
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
		CE_GrowingArray<CUI_Pin*>& inputPins = node->myInputs;
		for (CUI_Pin* input : inputPins)
		{
			if (input->OnDragBegin(aMessage))
			{
				CE_GrowingArray<CUI_Pin*>& connections = input->myConnections;
				if (connections.Size() > 0)
				{
					mySelectedPin = connections[0];
					mySelectedPin->myConnections.RemoveCyclic(input);
					connections.RemoveAll();
				}
				else
				{
					mySelectedPin = input;
				}

				aMessage.myData.Set<CUI_Pin*>(mySelectedPin);
				return true;
			}
		}

		CE_GrowingArray<CUI_Pin*>& outputPins = node->myOutPuts;
		for (CUI_Pin* output : outputPins)
		{
			if (output->OnDragBegin(aMessage))
			{
				mySelectedPin = output;
				aMessage.myData.Set<CUI_Pin*>(mySelectedPin);
				return true;
			}
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

	bool findInputPin = !pin1->IsInput();
	CUI_Pin* pin2 = GetDragEndPin(aMessage, findInputPin);
	if (!pin2)
		return false;

	if (findInputPin)
		ConnectPins(pin1->myNode->myID, pin1->myID, pin2->myNode->myID, pin2->myID);
	else
		ConnectPins(pin2->myNode->myID, pin2->myID, pin1->myNode->myID, pin1->myID);

	return true;
}

void CUI_NodeEditor::SaveGraph()
{
	SaveGraphToDisk(myFilePath.c_str());
}

void CUI_NodeEditor::DeleteSelectedNode()
{
	CUI_VisualNode* selectedNode = nullptr;
	for (CUI_VisualNode* node : myVisualNodes)
	{
		if (node->IsFocused())
		{
			selectedNode = node;
			break;
		}
	}

	if (selectedNode == nullptr)
		return;

	myVisualNodes.RemoveCyclic(selectedNode);
	DeleteWidget(selectedNode);
}

void CUI_NodeEditor::RenderNodeConnections(CE_RendererProxy& anRendererProxy, CUI_VisualNode* aNode)
{
	int numOutputs = aNode->myOutPuts.Size();
	float startOffset = 0.1f;
	float spacing = (1.f - startOffset * 2) / numOutputs;

	for (int i = 0; i < numOutputs; ++i)
	{
		float cutPoint = startOffset + spacing * 0.5f + spacing * i;

		const CUI_Pin* outputPin = aNode->myOutPuts[i];
		CE_Vector2f startPosition = outputPin->GetPosition();
		startPosition += outputPin->GetSize() * 0.5f;

		const CE_GrowingArray<CUI_Pin*>& connections = outputPin->GetConnections();
		for (const CUI_Pin* connection : connections)
		{
			CE_Vector2f endPosition = connection->GetPosition();
			endPosition += connection->GetSize() * 0.5f;

			RenderSteppedLine(anRendererProxy, startPosition, endPosition, cutPoint);
		}
	}
}

void CUI_NodeEditor::RenderSteppedLine(CE_RendererProxy& anRendererProxy, const CE_Vector2f& aStartPos, const CE_Vector2f& aEndPos, float aCutPoint)
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

CUI_Pin* CUI_NodeEditor::GetDragEndPin(CUI_DragMessage& aMessage, bool aGetInputPin)
{
	for (CUI_VisualNode* node : myVisualNodes)
	{
		CE_GrowingArray<CUI_Pin*>* pins = nullptr;
		if (aGetInputPin)
			pins = &node->myInputs;
		else
			pins = &node->myOutPuts;

		for (CUI_Pin* pin : *pins)
		{
			if (pin->OnDragEnd(aMessage))
				return pin;
		}
	}

	return nullptr;
}

CN_Node* CUI_NodeEditor::CreateRealNode(const char* aNodeType)
{
	CN_Node* node = nullptr;
	if (strcmp(aNodeType, "Init") == 0)
	{
		node = new CN_ScriptInitNode();
	}
	else if (strcmp(aNodeType, "Line Node") == 0)
	{
		node = new CN_ScriptDrawLineNode();
	}

	if (node)
		myRealNodes.Add(node);

	return node;
}

CUI_VisualNode* CUI_NodeEditor::CreateVisualNode(CN_Node* aRealNode)
{
	CUI_VisualNode* visualNode = new CUI_VisualNode(*myFont, aRealNode->myTempName.c_str());
	visualNode->myID = aRealNode->GetNodeID();

	myVisualNodes.Add(visualNode);
	AddWidget(visualNode);

	for (CN_Pin* realPin : aRealNode->myAllPins)
	{
		CUI_Pin* uiPin = new CUI_Pin(realPin->GetIsInput(), CUI_NodeEditor_priv::locPinSize, CUI_NodeEditor_priv::locDefaultPinColor);
		uiPin->myID = realPin->GetPinID();
		uiPin->myNode = visualNode;

		visualNode->AddPin(uiPin);
	}

	return visualNode;
}

CN_Node* CUI_NodeEditor::FindRealNode(u32 aNodeID)
{
	for (CN_Node* node : myRealNodes)
	{
		if (node->myNodeID == aNodeID)
			return node;
	}

	return nullptr;
}

CUI_VisualNode* CUI_NodeEditor::FindVisualNode(u32 aNodeID)
{
	for (CUI_VisualNode* node : myVisualNodes)
	{
		if (node->myID == aNodeID)
			return node;
	}

	return nullptr;
}

void CUI_NodeEditor::ConnectPins(u32 aOutputNode, u32 aOutputPin, u32 aInputNode, u32 aInputPin)
{
	CN_Node* realOutputNode = FindRealNode(aOutputNode);
	CN_Node* realInputNode = FindRealNode(aInputNode);

	CN_Pin* realOutputPin = realOutputNode->GetPin(aOutputPin);
	CN_Pin* realInputPin = realInputNode->GetPin(aInputPin);

	if (realInputPin->myConnectedPins.Size() == 0)
	{
		realInputPin->myConnectedPins.Add(realOutputPin);
		realOutputPin->myConnectedPins.Add(realInputPin);
	}


	CUI_VisualNode* visualOutputNode = FindVisualNode(aOutputNode);
	CUI_VisualNode* visualInputNode = FindVisualNode(aInputNode);

	CUI_Pin* visualOutputPin = visualOutputNode->GetPin(aOutputPin);
	CUI_Pin* visualInputPin = visualInputNode->GetPin(aInputPin);

	if (visualInputPin->myConnections.Size() == 0)
	{
		visualInputPin->myConnections.Add(visualOutputPin);
		visualOutputPin->myConnections.Add(visualInputPin);
	}
}

void CUI_NodeEditor::SaveGraphToDisk(const char* aFilePath)
{
	CE_BinaryFileWriter writer(aFilePath);
	if (!writer.IsOpen())
		return;

	// Write information about the nodes:
	// - Name (should not be needed, a NodeType-ID should be enough)
	// - ID (to differentiate nodes of the same type)
	// - Position (for UI)
	writer.Write(myVisualNodes.Size());
	for (CUI_VisualNode* node : myVisualNodes)
	{
		const CE_String& name = node->myLabel->GetText();
		int nameLenght = name.Lenght() + 2;
		writer.Write(nameLenght);
		writer.Write(name.c_str(), nameLenght);

		writer.Write(node->myID);
		writer.Write(node->GetPosition());
	}

	// Write information about the connections:
	// We only need to store connections in one directions, we're going to use output
	// so we write the following info for each output node:
		// For each pin:
		// - ID of the pin
			// For each connection:
			// - NodeID (ID of the node we're connected to)
			// - PinID (ID of the pin on the node we're connected to)

	for (CUI_VisualNode* node : myVisualNodes)
	{
		writer.Write(node->myID);

		CE_GrowingArray<CUI_Pin*> outputPins = node->myOutPuts;
		writer.Write(outputPins.Size());
		for (CUI_Pin* outputPin : outputPins)
		{
			writer.Write(outputPin->myID);
			writer.Write(outputPin->myConnections.Size());
			for (CUI_Pin* connection : outputPin->myConnections)
			{
				CUI_VisualNode* connectedNode = connection->myNode;
				writer.Write(connectedNode->myID);
				writer.Write(connection->myID);
			}
		}
	}
}

void CUI_NodeEditor::LoadGraph(const char* aFilePath)
{
	CE_BinaryFileReader reader(aFilePath);
	if (!reader.IsOpen())
		return;

	int nodeCount;
	reader.Read(nodeCount);
	for (int i = 0; i < nodeCount; ++i)
	{
		int nameLenght;
		reader.Read(nameLenght);

		char nameBuffer[64];
		reader.Read(nameBuffer, nameLenght);

		u32 nodeID;
		reader.Read(nodeID);
		if (myNextNodeID <= nodeID)
			myNextNodeID = nodeID + 1;

		CN_Node* realNode = CreateRealNode(nameBuffer);
		realNode->SetNodeID(nodeID);


		CE_Vector2f position;
		reader.Read(position);

		CUI_VisualNode* visualNode = CreateVisualNode(realNode);
		visualNode->SetPosition(position);
	}


	for (int i = 0; i < nodeCount; ++i)
	{
		u32 nodeID;
		reader.Read(nodeID);

		int pinCount;
		reader.Read(pinCount);
		for (int j = 0; j < pinCount; ++j)
		{
			u32 pinID;
			reader.Read(pinID);

			int connectionCount;
			reader.Read(connectionCount);
			for (int k = 0; k < connectionCount; ++k)
			{
				u32 connectedNodeID;
				u32 connectedPinID;

				reader.Read(connectedNodeID);
				reader.Read(connectedPinID);

				ConnectPins(nodeID, pinID, connectedNodeID, connectedPinID);
			}
		}
	}
}

void CUI_NodeEditor::OnNodeDropboxSelection(CUI_Widget* aWidget)
{
	myNodeDropbox->SetExpansion(false);
	myNodeDropbox->Hide();

	CUI_Label* label = static_cast<CUI_Label*>(aWidget);
	const CE_String& text = label->GetText();

	CN_Node* realNode = CreateRealNode(text.c_str());
	realNode->myNodeID = myNextNodeID++;

	CUI_VisualNode* visualNode = CreateVisualNode(realNode);
	visualNode->SetPosition(myMousePosition);
}
