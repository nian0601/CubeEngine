#pragma once
#include "CUI_Container.h"

class CUI_Label;
class CUI_Pin;

class CE_Font;

class CN_Node;

class CUI_VisualNode : public CUI_Container
{
	friend class CUI_NodeEditor;

public:
	CUI_VisualNode(const CE_Font& aFont, CN_Node* aRealNode);
	~CUI_VisualNode();

	void PrepareLayout() override;
	void Render(CE_RendererProxy& anRendererProxy) override;

	bool OnMouseMessage(const CUI_MouseMessage& aMessage) override;

	bool CanBeFocused() const override { return true; }

	//aNode will always be the input-node
	//aPinID will always be an output-pin on THIS node
	//aOtherPinID will always be an input-pin on the other node
	void ConnectWithNode(CUI_VisualNode* aNode, s32 aPinID, s32 aOtherPinID);
	void DisconnectPin(s32 aPinID);

	const unsigned int myID;
	int myNumOutputPins;

private:
	CUI_Pin* GetPin(u32 aID);

	CE_Vector4f myColor;
	CE_Vector2f myPositionOffset;

	CUI_Label* myLabel;

	CE_GrowingArray<CUI_Pin*> myPins;

	CN_Node* myRealNode;
};

