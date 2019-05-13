#pragma once

#include "CE_Any.h"

class CN_Node;

class CN_Pin
{
	friend class CUI_NodeEditor;
	friend class CUI_VisualNode;
	friend class CN_NodeGraph;
public:
	CN_Pin(u32 aDataType, u32 aPinID, bool aIsInput, const char* aName, CN_Node* aNode, const CE_Any& aDefaultvalue);

	void Execute();

	u32 GetPinID() const { return myPinID; }
	u32 GetDataType() const { return myDataType; }
	bool GetIsInput() const { return myIsInput; }
	const char* GetName() const { return myName.c_str(); }

	CN_Node* GetNode() { return myNode; }
	CE_GrowingArray<CN_Pin*>& GetConnectedPins() { return myConnectedPins; }


	template<typename T>
	void Write(const T& someData);

	template<typename T>
	T& Read();

protected:
	u32 myPinID;
	u32 myDataType;
	bool myIsInput;
	CE_Any myData;
	CE_String myName;

	CN_Node* myNode;
	CE_GrowingArray<CN_Pin*> myConnectedPins;
};

template<typename T>
void CN_Pin::Write(const T& someData)
{
	CE_ASSERT(CE_GetTypeID<T>() == GetDataType(), "Trying to set missmatching datatype on pin!");
	myData.Set(someData);
}

template<typename T>
T& CN_Pin::Read()
{
	CE_ASSERT(myIsInput, "Tried to read from an output pin, cant do that!");
	CE_ASSERT(myConnectedPins.Size() <= 1, "More than one connection to input-pin somehow..");

	if (myConnectedPins.Size() == 0)
		return myData.Get<T>();

	CN_Pin* pinOnConnectedNode = myConnectedPins[0];
	CE_ASSERT(CE_GetTypeID<T>() == pinOnConnectedNode->GetDataType(), "Trying to read missmatching datatype from pin!");
	pinOnConnectedNode->myNode->Execute(*pinOnConnectedNode);
	return pinOnConnectedNode->myData.Get<T>();

	//for (CN_Pin* input : myConnectedPins)
	//{
	//	CN_Node* node = input->myNode;
	//	node->Execute(*input);
	//}
	//return myData.Get<T>();
}