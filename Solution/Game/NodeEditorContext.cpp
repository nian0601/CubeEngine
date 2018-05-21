#include "stdafx.h"
#include "NodeEditorContext.h"

#include <CE_Engine.h>
#include <CE_Window.h>

#include <CUI_Manager.h>
#include <CUI_NodeEditor.h>

void NodeEditorContext::Init(CE_Engine& anEngine)
{
	CUI_NodeEditor* nodeEditor = new CUI_NodeEditor(anEngine.GetGPUContext());

	CE_Window& mainWindow = anEngine.GetMainWindow();
	mainWindow.GetUIManager().AddWidget(nodeEditor);
}

void NodeEditorContext::Update(float aDelta)
{
	aDelta;
}

void NodeEditorContext::Render()
{
}