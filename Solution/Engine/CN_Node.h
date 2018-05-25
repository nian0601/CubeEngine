#pragma once

class CN_Pin;
class CN_Node
{
	friend class CUI_NodeEditor;
public:
	CN_Node();
	virtual ~CN_Node();

	CN_Pin* GetPin(u32 aPinIndex);

	s32 GetNodeID() const { return myNodeID; }
	void SetNodeID(s32 aNodeID) { myNodeID = aNodeID; }

	CE_String myTempName;

protected:
	void AddPin(u32 aDataType, u32 aPinID, bool aIsInput);

private:
	u32 myNodeID;

	CE_GrowingArray<CN_Pin*> myInputPins;
	CE_GrowingArray<CN_Pin*> myOutputPins;

	CE_GrowingArray<CN_Pin*> myAllPins;
};

