#pragma once
class CN_Node;
class CN_Pin
{
	friend class CUI_NodeEditor;
	friend class CUI_VisualNode;
	friend class CN_NodeGraph;
public:
	CN_Pin(u32 aDataType, u32 aPinID, bool aIsInput, const char* aName, CN_Node* aNode);

	void Execute();

	u32 GetPinID() const { return myPinID; }
	u32 GetDataType() const { return myDataType; }
	bool GetIsInput() const { return myIsInput; }
	const char* GetName() const { return myName.c_str(); }

	CN_Node* GetNode() { return myNode; }
	CE_GrowingArray<CN_Pin*>& GetConnectedPins() { return myConnectedPins; }

private:
	u32 myDataType;
	u32 myPinID;
	bool myIsInput;
	CE_String myName;

	CN_Node* myNode;
	CE_GrowingArray<CN_Pin*> myConnectedPins;
};