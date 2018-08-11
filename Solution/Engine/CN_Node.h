#pragma once

class CN_Pin;
class CN_Node
{
	friend class CUI_NodeEditor;
	friend class CUI_VisualNode;
public:
	CN_Node();
	virtual ~CN_Node();

	virtual void Execute(const CN_Pin& anExecutingPin) = 0;

	CN_Pin* GetPin(u32 aPinIndex);

	s32 GetNodeID() const { return myNodeID; }
	void SetNodeID(s32 aNodeID) { myNodeID = aNodeID; }

	CE_String myTempName;

protected:
	CN_Pin* AddPin(u32 aDataType, u32 aPinID, bool aIsInput);

private:
	u32 myNodeID;

	CE_GrowingArray<CN_Pin*> myInputPins;
	CE_GrowingArray<CN_Pin*> myOutputPins;

	CE_GrowingArray<CN_Pin*> myAllPins;
};

