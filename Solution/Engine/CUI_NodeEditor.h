#pragma once

#include "CUI_Container.h"

#include "CE_Font.h"

class CUI_Pin;
class CUI_VisualNode;

class CE_GPUContext;
class CUI_NodeEditor : public CUI_Container
{
public:
	CUI_NodeEditor(CE_GPUContext& aGPUContext);
	~CUI_NodeEditor();

	void Render(CE_RendererProxy& anRendererProxy) override;

	bool OnMouseMessage(const CUI_MouseMessage& aMessage) override;
	bool OnDragBegin(CUI_DragMessage& aMessage) override;
	bool OnDragEnd(CUI_DragMessage& aMessage) override;

private:
	void RenderNodeConnections(CE_RendererProxy& anRendererProxy, CUI_VisualNode* aNode);
	void RenderSteppedLine(CE_RendererProxy& anRendererProxy, const CE_Vector2f& aStartPos, const CE_Vector2f& aEndPos, float aCutPoint);

	CUI_VisualNode* CreateNode(const char* aTitle, unsigned int anID);
	CUI_Pin* CreatePin(bool aIsInput, unsigned int anID);
	void CreateAndAddPin(CUI_VisualNode* aNode, bool aIsInput, unsigned int anID);
	void ConnectPins(CUI_Pin* aInputPin, CUI_Pin* aOutputPin);
	CUI_Pin* FindPin(unsigned int anID, bool aIsInput);
	CUI_Pin* FindPin(unsigned int anID);

	void SaveGraphToDisk(const char* aFilePath);
	void LoadGraph(const char* aFilePath);

	CE_Vector2f myMousePosition;
	CE_Font* myFont;

	CUI_Pin* mySelectedPin;

	unsigned int myNextPinID;
	unsigned int myNextNodeID;
	CE_GrowingArray<CUI_Pin*> myInputPins;
	CE_GrowingArray<CUI_Pin*> myOutputPins;
	CE_GrowingArray<CUI_VisualNode*> myNodes;
};

