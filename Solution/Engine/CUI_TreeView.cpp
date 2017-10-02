#include "stdafx.h"

#include "CUI_Button.h"
#include "CUI_TreeView.h"

CUI_TreeView::CUI_TreeView()
	: myIsExpanded(true)
{
	myButton = new CUI_Button({ 30.f, 30.f }, { 0.37f, 0.37f, 0.37f, 1.f });
	myButton->myOnClick = std::bind(&CUI_TreeView::OnToggleViewClick, this);

	AddWidget(myButton);
}

void CUI_TreeView::OnToggleViewClick()
{
	myIsExpanded = !myIsExpanded;

	for (CUI_Widget* widget : myWidgets)
	{
		if (myIsExpanded)
			widget->Show();
		else
			widget->Hide();
	}

	myButton->Show();
}
