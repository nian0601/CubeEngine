#pragma once
#include "CE_EngineDefines.h"
#include "CUI_Container.h"
#include "CE_Font.h"

class CUI_Pin;
class CUI_VisualNode;
class CUI_Dropbox;

class CE_GPUContext;

class CN_Node;
class CN_Pin;

class CUI_NodeEditor : public CUI_Container
{
public:
	CUI_NodeEditor(CE_GPUContext& aGPUContext);
	~CUI_NodeEditor();

	void Render(CE_RendererProxy& anRendererProxy) override;

	bool OnMouseMessage(const CUI_MouseMessage& aMessage) override;
	bool OnDragBegin(CUI_DragMessage& aMessage) override;
	bool OnDragEnd(CUI_DragMessage& aMessage) override;

	void SaveGraph();
	void DeleteSelectedNode();

private:
	void RenderNodeConnections(CE_RendererProxy& anRendererProxy, CUI_VisualNode* aNode);
	void RenderSteppedLine(CE_RendererProxy& anRendererProxy, const CE_Vector2f& aStartPos, const CE_Vector2f& aEndPos, float aCutPoint);


	CUI_Pin* GetDragEndPin(CUI_DragMessage& aMessage, bool aGetInputPin);

	CN_Node* CreateRealNode(const char* aNodeType);
	CUI_VisualNode* CreateVisualNode(CN_Node* aRealNode);

	CN_Node* FindRealNode(u32 aNodeID);
	CUI_VisualNode* FindVisualNode(u32 aNodeID);

	void ConnectPins(u32 aOutputNode, u32 aOutputPin, u32 aInputNode, u32 aInputPin);


	void SaveGraphToDisk(const char* aFilePath);
	void LoadGraph(const char* aFilePath);
	void OnNodeDropboxSelection(CUI_Widget* aWidget);

	CE_String myFilePath;

	CE_Vector2f myMousePosition;
	CE_Font* myFont;

	CUI_Pin* mySelectedPin;

	u32 myNextNodeID;
	CE_GrowingArray<CUI_VisualNode*> myVisualNodes;

	CUI_Dropbox* myNodeDropbox;

	CE_GrowingArray<CN_Node*> myRealNodes;
};

