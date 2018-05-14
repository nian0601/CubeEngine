#include "stdafx.h"
#include "NodeEditorContext.h"

#include <CE_Engine.h>
#include <CE_Font.h>
#include <CE_Window.h>

#include <CUI_Manager.h>
#include <CUI_NodeEditor.h>
#include <CUI_VisualNode.h>

NodeEditorContext::NodeEditorContext()
{
}


NodeEditorContext::~NodeEditorContext()
{
	CE_SAFE_DELETE(myFont);
}

void NodeEditorContext::Init(CE_Engine& anEngine)
{
	CE_Window& mainWindow = anEngine.GetMainWindow();

	myFont = new CE_Font();
	myFont->LoadFromFile("Data/Font/Decent_Font.png", anEngine.GetGPUContext());

	CUI_NodeEditor* nodeEditor = new CUI_NodeEditor();

	CUI_VisualNode* node = new CUI_VisualNode(*myFont, "TEST NODE");
	node->AddPin(true);
	node->AddPin(true);
	node->AddPin(false);
	node->AddPin(false);
	nodeEditor->AddWidget(node);

	CUI_VisualNode* node2 = new CUI_VisualNode(*myFont, "TEST NODE 2");
	node2->AddPin(true);
	node2->AddPin(true);
	nodeEditor->AddWidget(node2);

	mainWindow.GetUIManager().AddWidget(nodeEditor);
}

void NodeEditorContext::Update(float aDelta)
{
	aDelta;
}

void NodeEditorContext::Render()
{

}