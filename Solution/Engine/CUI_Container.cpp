#include "stdafx.h"
#include "CUI_Container.h"

CUI_Container::~CUI_Container()
{
	myWidgets.DeleteAll();
}

void CUI_Container::PrepareLayout()
{
	for (CUI_Widget* widget : myWidgets)
	{
		if (widget->IsVisible())
			widget->PrepareLayout();
	}
}

void CUI_Container::Render(CE_RendererProxy& anRendererProxy)
{
	for (CUI_Widget* widget : myWidgets)
	{
		if (widget->IsVisible())
			widget->Render(anRendererProxy);
	}
}

bool CUI_Container::OnMouseDown(const CUI_MouseMessage& aMessage)
{
	for (CUI_Widget* widget : myWidgets)
	{
		if (widget->OnMouseDown(aMessage))
			return true;
	}

	return CUI_Widget::OnMouseDown(aMessage);
}

bool CUI_Container::OnMouseUp(const CUI_MouseMessage& aMessage)
{
	for (CUI_Widget* widget : myWidgets)
	{
		if (widget->OnMouseUp(aMessage))
			return true;
	}

	return CUI_Widget::OnMouseUp(aMessage);
}

bool CUI_Container::OnMouseWheel(const CUI_MouseMessage& aMessage)
{
	for (CUI_Widget* widget : myWidgets)
	{
		if (widget->OnMouseWheel(aMessage))
			return true;
	}

	return CUI_Widget::OnMouseWheel(aMessage);
}

bool CUI_Container::OnMouseMove(const CUI_MouseMessage& aMessage)
{
	for (CUI_Widget* widget : myWidgets)
	{
		if (widget->OnMouseMove(aMessage))
			return true;
	}

	return CUI_Widget::OnMouseMove(aMessage);
}


bool CUI_Container::OnTextInput(const CE_WindowMessage& aMessage)
{
	for (CUI_Widget* widget : myWidgets)
	{
		if (widget->OnTextInput(aMessage))
			return true;
	}

	return CUI_Widget::OnTextInput(aMessage);
}

bool CUI_Container::OnDragBegin(CUI_DragMessage& aMessage)
{
	for (CUI_Widget* widget : myWidgets)
	{
		if (widget->OnDragBegin(aMessage))
			return true;
	}

	return CUI_Widget::OnDragBegin(aMessage);
}

bool CUI_Container::OnDragEnd(CUI_DragMessage& aMessage)
{
	for (CUI_Widget* widget : myWidgets)
	{
		if (widget->OnDragEnd(aMessage))
			return true;
	}

	return CUI_Widget::OnDragEnd(aMessage);
}

void CUI_Container::DeleteAllChildren()
{
	myWidgets.DeleteAll();
}

void CUI_Container::DeleteWidget(CUI_Widget* aWidget)
{
	myWidgets.DeleteCyclic(aWidget);
}

void CUI_Container::Show()
{
	for (CUI_Widget* widget : myWidgets)
		widget->Show();

	CUI_Widget::Show();
}

void CUI_Container::Hide()
{
	for (CUI_Widget* widget : myWidgets)
		widget->Hide();

	CUI_Widget::Hide();
}
