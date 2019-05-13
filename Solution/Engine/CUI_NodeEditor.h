#pragma once
#include "CE_EngineDefines.h"
#include "CUI_Container.h"
#include "CE_Font.h"

class CUI_Pin;
class CUI_VisualNode;
class CUI_Dropbox;
class CUI_HBox;
class CUI_VBox;

class CE_GPUContext;

class CN_Node;
class CN_Pin;
class CN_NodeGraph;

class CUI_NodeEditor : public CUI_Container
{
public:
	CUI_NodeEditor(CE_GPUContext& aGPUContext);
	~CUI_NodeEditor();

	void Render(CE_RendererProxy& anRendererProxy) override;

	bool OnMouseUp(const CUI_MouseMessage& aMessage) override;
	bool OnMouseMessage(const CUI_MouseMessage& aMessage) override;
	bool OnDragBegin(CUI_DragMessage& aMessage) override;
	bool OnDragEnd(CUI_DragMessage& aMessage) override;

	bool OnPinDragBegin(CUI_DragMessage& aMessage, CUI_Pin* aPin);

	void SaveGraph();
	void DeleteSelectedNode();

private:
	void RenderNodeConnections(CE_RendererProxy& anRendererProxy, CUI_VisualNode* aNode);
	void RenderSteppedLine(CE_RendererProxy& anRendererProxy, const CE_Vector2f& aStartPos, const CE_Vector2f& aEndPos, float aCutPoint);

	CUI_Pin* GetDragEndPin(CUI_DragMessage& aMessage);

	CUI_VisualNode* CreateVisualNode(CN_Node* aRealNode);

	void ConnectPins(CUI_Pin* aOutputPin, CUI_Pin* aInputPin);
	void ConnectPins(u32 aOutputNode, u32 aOutputPin, u32 aInputNode, u32 aInputPin);
	void ConnectVisualPins(u32 aOutputNode, u32 aOutputPin, u32 aInputNode, u32 aInputPin);

	void LoadGraph(const char* aFilePath);
	void CreateVisualNodesFromNodeGraph();
	void OnNodeDropboxSelection(CUI_Widget* aWidget, int aWidgetIndex);

	void SelectNode(CUI_VisualNode* aNode);

	CUI_HBox* CreateVectorWidget(const char* aText, CE_Vector2f& aVector);
	CUI_HBox* CreateFloatController(const char* aText, float& aValue);
	

	CE_String myFilePath;

	CE_Vector2f myMousePosition;
	CE_Font* myFont;

	CUI_Pin* mySelectedPin;
	CUI_VisualNode* mySelectedNode;

	u32 myNextNodeID;
	CE_GrowingArray<CUI_VisualNode*> myVisualNodes;

	CUI_Dropbox* myNodeDropbox;
	CUI_VBox* myContextualProperties;

	CN_NodeGraph* myNodeGraph;
};

