#include "stdafx.h"

#include "CN_Node.h"
#include "CN_NodeFactory.h"

#include "CN_ScriptInitNode.h"
#include "CN_ScriptDrawLineNode.h"
#include "CN_Script2In3OutNode.h"


CE_Map<CE_String, CN_NodeFactory::NodeInfo> CN_NodeFactory::myNodeInfos;
bool CN_NodeFactory::myHasRegisteredNodes = false;

void CN_NodeFactory::RegisterNodes()
{
	CE_ASSERT(myHasRegisteredNodes == false, "Nodes has allready been registered!");
	myHasRegisteredNodes = true;

	RegisterNode<CN_ScriptInitNode>("scriptInitNode", "Init");
	RegisterNode<CN_ScriptDrawLineNode>("scriptDrawLine", "Draw Line");
	RegisterNode<CN_Script2In3OutNode>("script2In3Out", "2 In - 3 Out");
}

CN_Node* CN_NodeFactory::CreateNode(const char* anIdentifier)
{
	CE_ASSERT(myNodeInfos.KeyExists(anIdentifier), "Node %s is not registered", anIdentifier);

	CN_Node* node = myNodeInfos[anIdentifier].myCtor();
	node->myIdentifier = anIdentifier;
	return node;
}

const char* CN_NodeFactory::GetNodeScreenName(const char* anIdentifier)
{
	CE_ASSERT(myNodeInfos.KeyExists(anIdentifier), "Node %s is not registered", anIdentifier);
	return myNodeInfos[anIdentifier].myScreenName.c_str();
}

CN_NodeFactory::CN_NodeFactory()
{
}


CN_NodeFactory::~CN_NodeFactory()
{
}
