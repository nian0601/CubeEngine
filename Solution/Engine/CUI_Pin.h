#pragma once
#include "CUI_Widget.h"

class CE_Font;

class CUI_VisualNode;
class CUI_Label;
class CUI_Image;

class CN_Pin;

class CUI_Pin : public CUI_Widget
{
	friend class CUI_NodeEditor;
	friend class CUI_VisualNode;

public:
	CUI_Pin(CUI_VisualNode& aNode, CN_Pin* aRealPin, const CE_Font& aFont);

	void PrepareLayout() override;
	void Render(CE_RendererProxy& anRendererProxy) override;

	bool OnDragMessage(CUI_DragMessage& aMessage) override;

	bool IsInput() const { return myIsInput; }
	const CE_GrowingArray<CUI_Pin*>& GetConnections() const { return myConnections; }
	bool IsConnected() const { return myConnections.Size() > 0; }

	float GetPinWidth() const;
	unsigned int GetDataType() const;

	unsigned int myID;
private:
	CE_Vector4f myColor;
	bool myIsInput;

	CUI_VisualNode& myNode;
	CUI_Label* myLabel;
	CUI_Image* myImage;
	CE_GrowingArray<CUI_Pin*> myConnections;
	CN_Pin* myRealPin;
};
