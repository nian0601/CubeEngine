#include "stdafx.h"
#include "CUI_Manager.h"
#include "CUI_WIdget.h"
#include "CE_Input.h"

CUI_Manager::CUI_Manager(CE_Input& anInput)
	: myInput(anInput)
{
	myWindowSize.x = 1280.f;
	myWindowSize.y = 720.f;
}

CUI_Manager::~CUI_Manager()
{
	myWidgets.DeleteAll();
}

void CUI_Manager::AddWidget(CUI_Widget* anWidget)
{
	myWidgets.Add(anWidget);
}

void CUI_Manager::Update()
{
	for (CUI_Widget* widget : myWidgets)
	{
		widget->SetWindowSize(myWindowSize);
		widget->PrepareLayout();
	}

	for (CUI_Widget* widget : myWidgets)
	{
		widget->FinalizeLayout();
	}

	if (myInput.MouseDown(0))
	{
		CE_Vector2f mousePos = myInput.GetMousePosition();
		for (CUI_Widget* widget : myWidgets)
		{
			if (widget->OnClick(mousePos))
				return;
		}
	}
}

void CUI_Manager::Render(CE_RendererProxy& anRendererProxy)
{
	for (CUI_Widget* widget : myWidgets)
	{
		widget->Render(anRendererProxy);
	}
}
