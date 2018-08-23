#pragma once

#define CN_DEFINE_NODE(NODE_CLASS) \
private:\
friend class CN_NodeFactory;\
static CN_Node* CreateNode() { return new NODE_CLASS(); }\

#include "CN_Pin.h"
#include "CE_TypeID.h"

class CN_Node
{
	friend class CUI_NodeEditor;
	friend class CUI_VisualNode;
	friend class CN_NodeFactory;
public:
	CN_Node();
	virtual ~CN_Node();

	virtual void Execute(const CN_Pin& anExecutingPin) = 0;

	CN_Pin* GetPin(u32 aPinIndex);

	s32 GetNodeID() const { return myNodeID; }
	void SetNodeID(s32 aNodeID) { myNodeID = aNodeID; }
	CE_String myTempName;

	const char* GetIdentifier() const { return myIdentifier.c_str(); }

protected:
	template <typename T>
	CN_Pin* AddPin(u32 aPinID, bool aIsInput);

private:
	u32 myNodeID;
	CE_String myIdentifier;

	CE_GrowingArray<CN_Pin*> myAllPins;
};


template <typename T>
CN_Pin* CN_Node::AddPin(u32 aPinID, bool aIsInput)
{
	CN_Pin* pin = new CN_Pin(CE_GetTypeID<T>(), aPinID, aIsInput, this);
	myAllPins.Add(pin);

	return pin;
}