#include "stdafx.h"
#include "CUI_Container.h"

CUI_Container::~CUI_Container()
{
	myWidgets.DeleteAll();
}

void CUI_Container::Render(CE_RendererProxy& anRendererProxy)
{
	for (CUI_Widget* widget : myWidgets)
	{
		if (widget->IsVisible())
			widget->Render(anRendererProxy);
	}
}

bool CUI_Container::OnClick(const CE_Vector2f& aMousePosition)
{
	for (CUI_Widget* widget : myWidgets)
	{
		if (widget->IsVisible() && widget->OnClick(aMousePosition))
			return true;
	}

	return false;
}
