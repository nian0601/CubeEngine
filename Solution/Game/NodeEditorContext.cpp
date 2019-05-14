#include "stdafx.h"
#include "NodeEditorContext.h"

#include <CE_Engine.h>
#include <CE_Input.h>
#include <CE_Window.h>

#include <CUI_Manager.h>
#include <CUI_NodeEditor.h>

void NodeEditorContext::Init(CE_Engine& anEngine)
{
	myNodeEditor = new CUI_NodeEditor();

	CE_Window& mainWindow = anEngine.GetMainWindow();
	mainWindow.GetUIManager().AddWidget(myNodeEditor);

	myInput = &anEngine.GetInput();
}

void NodeEditorContext::Update(float aDelta)
{
	aDelta;

	if (myInput->KeyIsPressed(DIK_LCONTROL) && myInput->KeyDown(DIK_S))
		myNodeEditor->SaveGraph();

	if (myInput->KeyDown(DIK_DELETE))
		myNodeEditor->DeleteSelectedNode();
}

void NodeEditorContext::Render()
{
}