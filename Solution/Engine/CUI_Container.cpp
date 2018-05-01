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

void CUI_Container::OnMouseDown(const CE_Vector2f& aMousePosition)
{
	CUI_Widget::OnMouseDown(aMousePosition);

	for (CUI_Widget* widget : myWidgets)
		widget->OnMouseDown(aMousePosition);
}

bool CUI_Container::OnMouseUp(const CE_Vector2f& aMousePosition)
{
	for (CUI_Widget* widget : myWidgets)
	{
		if (widget->OnMouseUp(aMousePosition))
			return true;
	}

	return CUI_Widget::OnMouseUp(aMousePosition);
}

void CUI_Container::OnMouseMove(const CE_Vector2f& aNewMousePosition, const CE_Vector2f& aOldMousePosition)
{
	for (CUI_Widget* widget : myWidgets)
		widget->OnMouseMove(aNewMousePosition, aOldMousePosition);
}

void CUI_Container::DeleteAllChildren()
{
	myWidgets.DeleteAll();
}
