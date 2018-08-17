#pragma once
#include "CE_Map.h"

class CN_Node;

class CN_NodeFactory
{
	// FunctionPtr called NodeCtor:
	// Returns a CN_Node*
	// Takes no parameters
	typedef CN_Node* (*NodeCtor)();

public:
	struct NodeInfo
	{
		NodeCtor myCtor;
		CE_String myIdentifier;
		CE_String myScreenName;
	};

	static void RegisterNodes();

	template <typename T>
	static void RegisterNode(const char* anIdentifier, const char* anScreenName);

	static CN_Node* CreateNode(const char* anIdentifier);
	static const char* GetNodeScreenName(const char* anIdentifier);

private:
	CN_NodeFactory();
	~CN_NodeFactory();

	static CE_Map<CE_String, NodeInfo> myNodeInfos;
	static bool myHasRegisteredNodes;
};


template <typename T>
void CN_NodeFactory::RegisterNode(const char* anIdentifier, const char* anScreenName)
{
	CE_ASSERT(myNodeInfos.KeyExists(anIdentifier) == false, "Node %s allready registered", anScreenName);

	NodeInfo info;
	info.myCtor = &T::CreateNode;
	info.myIdentifier = anIdentifier;
	info.myScreenName = anScreenName;

	myNodeInfos[anIdentifier] = info;
}
