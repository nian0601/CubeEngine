#include "stdafx.h"
#include "CUI_Manager.h"
#include "CUI_WIdget.h"

CUI_Manager::~CUI_Manager()
{
	myWidgets.DeleteAll();
}

void CUI_Manager::AddWidget(CUI_Widget* anWidget)
{
	myWidgets.Add(anWidget);
}

void CUI_Manager::Render(CE_RendererProxy& anRendererProxy)
{
	for (CUI_Widget* widget : myWidgets)
	{
		widget->PrepareLayout();
	}

	for (CUI_Widget* widget : myWidgets)
	{
		widget->Render(anRendererProxy);
	}
}
