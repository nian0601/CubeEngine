#include "stdafx.h"
#include "CN_NodeGraph.h"
#include "CE_BinaryFileReader.h"
#include "CN_Node.h"
#include "CN_NodeFactory.h"
#include "CUI_Pin.h"
#include "CE_BinaryFileWriter.h"


#include "CN_ScriptInitNode.h"


CN_NodeGraph::CN_NodeGraph()
	: myInitNode(nullptr)
{
}


CN_NodeGraph::~CN_NodeGraph()
{
	myNodes.DeleteAll();
}

void CN_NodeGraph::Execute()
{
	if (myInitNode)
	{
		// This should obviously not be casted into a specific node..
		// The only reason I do it now is that the normal Execute-funtion expects an "executing pin"-parameter,
		// I should replace that with a blackboard (need one anyway for BT-nodes), and just put pin-id in there.
		static_cast<CN_ScriptInitNode*>(myInitNode)->Execute();
	}
}

void CN_NodeGraph::Load(const char* aFilePath)
{
	CE_BinaryFileReader reader(aFilePath);
	if (!reader.IsOpen())
		return;

	myMaxNodeID = 0;

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
		if (myMaxNodeID <= nodeID)
			myMaxNodeID = nodeID + 1;

		CN_Node* node = CN_NodeFactory::CreateNodeFromIdentifier(nameBuffer);
		node->SetNodeID(nodeID);

		reader.Read(node->myPosition);
		
		myNodes.Add(node);

		if (node->IsInitNode())
		{
			//CE_ASSERT(myInitNode == nullptr, "NodeGraph [%s] has multiple init-nodes, only one is supported for now.", myFilePath.c_str());
			myInitNode = node;
		}
	}


	for (int i = 0; i < nodeCount; ++i)
	{
		u32 nodeID;
		reader.Read(nodeID);

		CN_Node* node = GetNode(nodeID);
		
		for (CN_Pin* pin : node->myAllPins)
		{
			u32 pinID;
			reader.Read(pinID);

			// Need a nicer way of handling this, just disconnect the node and set default-vaules on all pins?
			CE_ASSERT(pinID == pin->GetPinID(), "Pins changed ID since the graph was saved");

			if (pin->GetIsInput())
			{
				bool isConst;
				reader.Read(isConst);

				if (isConst)
				{
					reader.Read(pin->myData);
				}
			}
			else
			{
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
}

void CN_NodeGraph::Save(const char* aFilePath)
{
	CE_BinaryFileWriter writer(aFilePath);
	if (!writer.IsOpen())
		return;

	// Write information about the nodes:
	// - Name (should not be needed, a NodeType-ID should be enough)
	// - ID (to differentiate nodes of the same type)
	// - Position (for UI)
	writer.Write(myNodes.Size());
	for (CN_Node* node : myNodes)
	{
		const CE_String& name = node->GetIdentifier();
		int nameLenght = name.Lenght() + 2;
		writer.Write(nameLenght);
		writer.Write(name.c_str(), nameLenght);

		writer.Write(node->GetNodeID());
		writer.Write(node->myPosition);
	}

	// Write information about the connections:
	// We only need to store connections in one directions, we're going to use output
	// so we write the following info for each output node:
	// For each pin:
	// - ID of the pin
	// For each connection:
	// - NodeID (ID of the node we're connected to)
	// - PinID (ID of the pin on the node we're connected to)

	for (CN_Node* node : myNodes)
	{
		writer.Write(node->GetNodeID());

		for (CN_Pin* pin : node->myAllPins)
		{
			writer.Write(pin->GetPinID());

			if (pin->GetIsInput())
			{
				if (pin->GetConnectedPins().Size() > 0)
				{
					writer.Write(false);
				}
				else
				{
					writer.Write(true);
					writer.Write(pin->myData);
				}
			}
			else
			{
				writer.Write(pin->myConnectedPins.Size());
				for (CN_Pin* connection : pin->myConnectedPins)
				{
					CN_Node* connectedNode = connection->myNode;
					writer.Write(connectedNode->GetNodeID());
					writer.Write(connection->GetPinID());
				}
			}
		}
	}
}

void CN_NodeGraph::AddNode(CN_Node* aNode)
{
	myNodes.Add(aNode);
}

void CN_NodeGraph::DeleteNode(CN_Node* aNode)
{
	myNodes.DeleteCyclic(aNode);
}

void CN_NodeGraph::ConnectPins(u32 aOutputNode, u32 aOutputPin, u32 aInputNode, u32 aInputPin)
{
	CN_Node* outputNode = nullptr;
	CN_Node* inputNode = nullptr;

	for (CN_Node* node : myNodes)
	{
		if (node->GetNodeID() == aOutputNode)
			outputNode = node;
		else if (node->GetNodeID() == aInputNode)
			inputNode = node;
	}

	CN_Pin* outputPin = outputNode->GetPin(aOutputPin);
	CN_Pin* inputPin = inputNode->GetPin(aInputPin);

	if (inputPin->myConnectedPins.Size() == 0)
	{
		inputPin->myConnectedPins.Add(outputPin);
		outputPin->myConnectedPins.Add(inputPin);
	}
}

CN_Node* CN_NodeGraph::GetNode(u32 aNodeID)
{
	for (CN_Node* node : myNodes)
	{
		if (node->GetNodeID() == aNodeID)
			return node;
	}

	return nullptr;
}
