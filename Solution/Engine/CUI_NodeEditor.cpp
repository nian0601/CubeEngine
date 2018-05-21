#include "stdafx.h"
#include "CUI_NodeEditor.h"
#include "CUI_VisualNode.h"
#include "CUI_Pin.h"
#include "CUI_Message.h"
#include "CE_RendererProxy.h"
#include "CE_BinaryFileWriter.h"
#include "CE_BinaryFileReader.h"
#include "CUI_Label.h"

namespace CUI_NodeEditor_priv
{
	const CE_Vector2f locPinSize = CE_Vector2f(16.f, 16.f);
	const CE_Vector4f locDefaultPinColor = CE_Vector4f(0.2f, 0.2f, 0.2f, 1.f);
}

#define SAVE_GRAPHS 0
CUI_NodeEditor::CUI_NodeEditor(CE_GPUContext& aGPUContext)
{
	myNextPinID = 0;
	myNextNodeID = 0;

	mySize.x = -1.f;
	mySize.y = -1.f;

	myFont = new CE_Font();
	myFont->LoadFromFile("Data/Font/Decent_Font.png", aGPUContext);

#if SAVE_GRAPHS == 1
	CUI_VisualNode* node = CreateNode("TEST NODE", myNextNodeID++);
	node->SetPosition({ 100.f, 100.f });
	CreateAndAddPin(node, true, myNextPinID++);
	CreateAndAddPin(node, true, myNextPinID++);
	CreateAndAddPin(node, false, myNextPinID++);
	CreateAndAddPin(node, false, myNextPinID++);

	CUI_VisualNode* node2 = CreateNode("TEST NODE 2", myNextNodeID++);
	node2->SetPosition({ 300.f, 100.f });
	CreateAndAddPin(node2, true, myNextPinID++);
	CreateAndAddPin(node2, false, myNextPinID++);
	CreateAndAddPin(node2, true, myNextPinID++);

	CUI_VisualNode* node3 = CreateNode("TEST NODE 3", myNextNodeID++);
	node3->SetPosition({ 300.f, 200.f });
	CreateAndAddPin(node3, true, myNextPinID++);
	CreateAndAddPin(node3, true, myNextPinID++);
	CreateAndAddPin(node3, false, myNextPinID++);
	CreateAndAddPin(node3, true, myNextPinID++);

	CUI_VisualNode* node4 = CreateNode("TEST NODE 4", myNextNodeID++);
	node4->SetPosition({ 300.f, 200.f });
	CreateAndAddPin(node4, false, myNextPinID++);
#else
	LoadGraph("testgraph.cegraph");
#endif
	
}

CUI_NodeEditor::~CUI_NodeEditor()
{
	CE_SAFE_DELETE(myFont);
}

void CUI_NodeEditor::Render(CE_RendererProxy& anRendererProxy)
{
	anRendererProxy;
	CUI_Container::Render(anRendererProxy);

	for (CUI_VisualNode* node : myNodes)
		RenderNodeConnections(anRendererProxy, node);

	if (mySelectedPin)
	{
		CE_Vector2f startPos = mySelectedPin->GetPosition();
		startPos += CUI_NodeEditor_priv::locPinSize * 0.5f;

		RenderSteppedLine(anRendererProxy, startPos, myMousePosition, 0.5f);
	}

#if SAVE_GRAPHS == 1
	static int counter = 0;
	counter++;
	if (counter > 1000) 
	{
		counter = 0;
		SaveGraphToDisk("testgraph.cegraph");
	}
#endif
}

bool CUI_NodeEditor::OnMouseMessage(const CUI_MouseMessage& aMessage)
{
	if (aMessage.myType == CUI_MouseMessage::MOUSE_MOVE)
	{
		myMousePosition = aMessage.myNewPosition;
	}

	return false;
}

bool CUI_NodeEditor::OnDragBegin(CUI_DragMessage& aMessage)
{
	mySelectedPin = nullptr;

	for (CUI_Pin* input : myInputPins)
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

	for (CUI_Pin* output : myOutputPins)
	{
		if (output->OnDragBegin(aMessage))
		{
			mySelectedPin = output;
			aMessage.myData.Set<CUI_Pin*>(mySelectedPin);
			return true;
		}
	}

	return false;
}

bool CUI_NodeEditor::OnDragEnd(CUI_DragMessage& aMessage)
{
	if (!mySelectedPin)
		return false;

	CUI_Pin* prevSelected = mySelectedPin;
	mySelectedPin = nullptr;

	if (prevSelected->IsInput())
	{
		for (CUI_Pin* output : myOutputPins)
		{
			if (output->OnDragEnd(aMessage))
			{
				ConnectPins(prevSelected, output);
				prevSelected = nullptr;
				return true;
			}
		}
	}
	else
	{
		for (CUI_Pin* input : myInputPins)
		{
			if (input->OnDragEnd(aMessage))
			{
				ConnectPins(input, prevSelected);
				prevSelected = nullptr;
				return true;
			}
		}
	}

	return false;
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

CUI_VisualNode* CUI_NodeEditor::CreateNode(const char* aTitle, unsigned int anID)
{
	CUI_VisualNode* node = new CUI_VisualNode(*myFont, aTitle);
	node->myID = anID;

	myNodes.Add(node);
	AddWidget(node);

	return node;
}

CUI_Pin* CUI_NodeEditor::CreatePin(bool aIsInput, unsigned int anID)
{
	CUI_Pin* pin = new CUI_Pin(aIsInput, CUI_NodeEditor_priv::locPinSize, CUI_NodeEditor_priv::locDefaultPinColor);
	pin->myID = anID;

	if (aIsInput)
		myInputPins.Add(pin);
	else
		myOutputPins.Add(pin);

	return pin;
}

void CUI_NodeEditor::CreateAndAddPin(CUI_VisualNode* aNode, bool aIsInput, unsigned int anID)
{
	CUI_Pin* pin = CreatePin(aIsInput, anID);
	aNode->AddPin(pin);
}

void CUI_NodeEditor::ConnectPins(CUI_Pin* aInputPin, CUI_Pin* aOutputPin)
{
	if (aInputPin->myConnections.Size() == 0)
	{
		aInputPin->myConnections.Add(aOutputPin);
		aOutputPin->myConnections.Add(aInputPin);
	}
}

CUI_Pin* CUI_NodeEditor::FindPin(unsigned int anID, bool aIsInput)
{
	CUI_Pin* pin = FindPin(anID);
	if (!pin)
	{
		CE_ASSERT(false, "Failed to find inputpin with ID: %d", anID);
		return nullptr;
	}

	CE_ASSERT(pin->myIsInput == aIsInput, "Missmatching pin ID and input/output-type")
	return pin;
}

CUI_Pin* CUI_NodeEditor::FindPin(unsigned int anID)
{
	for (CUI_Pin* input : myInputPins)
	{
		if (input->myID == anID)
			return input;
	}

	for (CUI_Pin* output : myOutputPins)
	{
		if (output->myID == anID)
			return output;
	}

	return nullptr;
}

void CUI_NodeEditor::SaveGraphToDisk(const char* aFilePath)
{
	CE_BinaryFileWriter writer(aFilePath);

	writer.Write(myInputPins.Size());
	for (CUI_Pin* input : myInputPins)
		writer.Write(input->myID);

	writer.Write(myOutputPins.Size());
	for (CUI_Pin* output : myOutputPins)
	{
		writer.Write(output->myID);

		writer.Write(output->myConnections.Size());
		for (CUI_Pin* connection : output->myConnections)
			writer.Write(connection->myID);
	}

	writer.Write(myNodes.Size());
	for (CUI_VisualNode* node : myNodes)
	{
		const CE_String& name = node->myLabel->GetText();
		int nameLenght = name.Lenght() + 2;
		writer.Write(nameLenght);
		writer.Write(name.c_str(), nameLenght);

		writer.Write(node->myID);
		writer.Write(node->GetPosition());

		writer.Write(node->myPins.Size());
		for (CUI_Pin* pin : node->myPins)
			writer.Write(pin->myID);
	}
}

void CUI_NodeEditor::LoadGraph(const char* aFilePath)
{
	CE_BinaryFileReader reader(aFilePath);

	int inputCount;
	reader.Read(inputCount);
	for (int i = 0; i < inputCount; ++i)
	{
		unsigned int pinID;
		reader.Read(pinID);

		CreatePin(true, pinID);

		if (myNextPinID <= pinID)
			myNextPinID = pinID + 1;
	}

	int outputCount;
	reader.Read(outputCount);
	for (int i = 0; i < outputCount; ++i)
	{
		unsigned int pinID;
		reader.Read(pinID);

		CUI_Pin* outputPin = CreatePin(false, pinID);
		if (myNextPinID <= pinID)
			myNextPinID = pinID + 1;

		int connectionCount;
		reader.Read(connectionCount);
		for (int j = 0; j < connectionCount; ++j)
		{
			unsigned int connectionID;
			reader.Read(connectionID);

			CUI_Pin* inputPin = FindPin(connectionID, true);

			outputPin->myConnections.Add(inputPin);
			inputPin->myConnections.Add(outputPin);
		}
	}

	int nodeCount;
	reader.Read(nodeCount);
	for (int i = 0; i < nodeCount; ++i)
	{
		int nameLenght;
		reader.Read(nameLenght);

		char nameBuffer[64];
		reader.Read(nameBuffer, nameLenght);

		unsigned int nodeID;
		reader.Read(nodeID);

		CE_Vector2f position;
		reader.Read(position);

		CUI_VisualNode* node = CreateNode(nameBuffer, nodeID);
		node->SetPosition(position);

		int nodePinCount;
		reader.Read(nodePinCount);
		for (int j = 0; j < nodePinCount; ++j)
		{
			unsigned int pinID;
			reader.Read(pinID);

			CUI_Pin* pin = FindPin(pinID);
			node->AddPin(pin);
		}
	}
}
