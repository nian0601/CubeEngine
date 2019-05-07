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
	friend class CN_NodeGraph;

public:
	CN_Node();
	virtual ~CN_Node();

	virtual void Execute(const CN_Pin& anExecutingPin) = 0;

	CN_Pin* GetPin(u32 aPinIndex);

	u32 GetNodeID() const { return myNodeID; }
	void SetNodeID(u32 aNodeID) { myNodeID = aNodeID; }

	const char* GetIdentifier() const { return myIdentifier.c_str(); }

	virtual bool IsInitNode() const { return false; }


	template <typename T>
	void Write(u32 aPinID, const T& someData);

	template <typename T>
	T& Read(u32 aPinID);

protected:
	template <typename T>
	CN_Pin* AddPin(u32 aPinID, bool aIsInput, const char* aName);

private:
	u32 myNodeID;
	CE_String myIdentifier;
	CE_Vector2f myPosition;

	CE_GrowingArray<CN_Pin*> myAllPins;
};


template <typename T>
void CN_Node::Write(u32 aPinID, const T& someData)
{
	if(CN_Pin* pin = GetPin(aPinID))
		pin->Write(someData);
}

template <typename T>
T& CN_Node::Read(u32 aPinID)
{
	if (CN_Pin* pin = GetPin(aPinID))
		return pin->Read();

	return T();
}

template <typename T>
CN_Pin* CN_Node::AddPin(u32 aPinID, bool aIsInput, const char* aName)
{
	CN_Pin* pin = new CN_Pin(CE_GetTypeID<T>(), aPinID, aIsInput, aName, this);
	myAllPins.Add(pin);

	return pin;
}