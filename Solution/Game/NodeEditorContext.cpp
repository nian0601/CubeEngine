#include "stdafx.h"
#include "NodeEditorContext.h"

#include <CE_Engine.h>
#include <CE_Window.h>

#include <CUI_Manager.h>
#include <CUI_NodeEditor.h>
#include <CUI_VisualNode.h>

NodeEditorContext::NodeEditorContext()
{
}


NodeEditorContext::~NodeEditorContext()
{
}

void NodeEditorContext::Init(CE_Engine& anEngine)
{
	CE_Window& mainWindow = anEngine.GetMainWindow();

	CUI_NodeEditor* nodeEditor = new CUI_NodeEditor();

	CUI_VisualNode* node = new CUI_VisualNode();
	node->AddPin(true);
	node->AddPin(true);
	node->AddPin(false);
	node->AddPin(false);
	nodeEditor->AddWidget(node);

	mainWindow.GetUIManager().AddWidget(nodeEditor);
}

void NodeEditorContext::Update(float aDelta)
{
	aDelta;
}

void NodeEditorContext::Render()
{

}