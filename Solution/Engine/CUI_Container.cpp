#include "stdafx.h"
#include "CUI_Container.h"

CUI_Container::~CUI_Container()
{
	myWidgets.DeleteAll();
}

void CUI_Container::FinalizeLayout()
{
	for (CUI_Widget* widget : myWidgets)
		widget->FinalizeLayout();
}

void CUI_Container::Render(CE_RendererProxy& anRendererProxy)
{
	for (CUI_Widget* widget : myWidgets)
		widget->Render(anRendererProxy);
}

void CUI_Container::SetWindowSize(const CE_Vector2f& aSize)
{
	for (CUI_Widget* widget : myWidgets)
		widget->SetWindowSize(aSize);
}

bool CUI_Container::OnClick(const CE_Vector2f& aMousePosition)
{
	for (CUI_Widget* widget : myWidgets)
	{
		if (widget->OnClick(aMousePosition))
			return true;
	}

	return false;
}
