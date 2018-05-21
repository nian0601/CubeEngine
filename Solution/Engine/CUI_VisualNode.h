#pragma once
#include "CUI_Container.h"

class CUI_Label;
class CUI_Pin;

class CE_Font;

class CUI_VisualNode : public CUI_Container
{
	friend class CUI_NodeEditor;

public:
	CUI_VisualNode(const CE_Font& aFont, const char* aLabel);
	~CUI_VisualNode();

	void PrepareLayout() override;
	void Render(CE_RendererProxy& anRendererProxy) override;

	bool OnMouseMessage(const CUI_MouseMessage& aMessage) override;

	bool CanBeFocused() const override { return true; }

	void AddPin(CUI_Pin* aPin);

	unsigned int myID;
private:
	CE_Vector4f myColor;
	CE_Vector2f myPositionOffset;

	float myPinSize;
	CUI_Label* myLabel;

	CE_GrowingArray<CUI_Pin*> myInputs;
	CE_GrowingArray<CUI_Pin*> myOutPuts;

	//Used only for saving and loading for now
	CE_GrowingArray<CUI_Pin*> myPins;
};

