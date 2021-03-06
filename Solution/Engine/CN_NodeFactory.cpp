#include "stdafx.h"

#include "CN_Node.h"
#include "CN_NodeFactory.h"

#include "CN_ScriptInitNode.h"
#include "CN_ScriptDrawLineNode.h"
#include "CN_Script3In3OutNode.h"
#include "CN_PositionNodes.h"


CE_Map<CE_String, CN_NodeFactory::NodeInfo> CN_NodeFactory::myNodeInfos;
bool CN_NodeFactory::myHasRegisteredNodes = false;

void CN_NodeFactory::RegisterNodes()
{
	CE_ASSERT(myHasRegisteredNodes == false, "Nodes has allready been registered!");
	myHasRegisteredNodes = true;

	RegisterNode<CN_ScriptInitNode>("scriptInitNode", "Init");
	RegisterNode<CN_ScriptDrawLineNode>("scriptDrawLine", "Draw Line");
	RegisterNode<CN_Script3In3OutNode>("script3In3Out", "3 In - 3 Out");
	RegisterNode<CN_Position1Node>("position1Node", "Position 1");
	RegisterNode<CN_Position2Node>("position2Node", "Position 2");
	RegisterNode<CN_Position3Node>("position3Node", "Position 3");
	RegisterNode<CN_Position4Node>("position4Node", "Position 4");
}

CN_Node* CN_NodeFactory::CreateNodeFromIdentifier(const char* anIdentifier)
{
	CE_ASSERT(myNodeInfos.KeyExists(anIdentifier), "Node %s is not registered", anIdentifier);

	return CreateNodeInternal(myNodeInfos[anIdentifier]);
}

CN_Node* CN_NodeFactory::CreateNodeFromScreenName(const char* anScreenName)
{
	for (auto it = myNodeInfos.Begin(); it != myNodeInfos.End(); it = myNodeInfos.Next(it))
	{
		if (it.Second().myScreenName == anScreenName)
			return CreateNodeInternal(it.Second());
	}

	CE_ASSERT(false, "Node %s is not registered", anScreenName);
	return nullptr;
}

const char* CN_NodeFactory::GetNodeScreenName(const char* anIdentifier)
{
	CE_ASSERT(myNodeInfos.KeyExists(anIdentifier), "Node %s is not registered", anIdentifier);
	return myNodeInfos[anIdentifier].myScreenName.c_str();
}

void CN_NodeFactory::GetNodeScreenNames(CE_GrowingArray<CE_String>& outNames)
{
	for (auto it = myNodeInfos.Begin(); it != myNodeInfos.End(); it = myNodeInfos.Next(it))
	{
		outNames.Add(it.Second().myScreenName);
	}
}

CN_Node* CN_NodeFactory::CreateNodeInternal(const NodeInfo& someNodeInfo)
{
	CN_Node* node = someNodeInfo.myCtor();
	node->myIdentifier = someNodeInfo.myIdentifier;
	return node;
}
