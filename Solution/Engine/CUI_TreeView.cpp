#include "stdafx.h"

#include "CUI_Button.h"
#include "CUI_TreeView.h"

CUI_TreeView::CUI_TreeView(const CE_String& aString)
	: myIsExpanded(false)
{
	myButton = new CUI_Button(aString);
	myButton->myOnClick = std::bind(&CUI_TreeView::OnToggleViewClick, this);

	AddWidget(myButton);
}

void CUI_TreeView::PrepareLayout()
{
	if (myWidgets.Size() == 0)
		return;

	CE_Vector2f position = GetPosition();
	mySize = CE_Vector2f(0.f);

	myWidgets[0]->SetPosition(position);
	myWidgets[0]->PrepareLayout();

	CE_Vector2f childSize = myWidgets[0]->GetSize();
	position.y += childSize.y;
	position.x += 10.f;

	UpdateSelfSize(myWidgets[0]);

	for (int i = 1; i < myWidgets.Size(); ++i)
	{
		CUI_Widget* child = myWidgets[i];
		child->Show();
		if (!myIsExpanded)
			child->Hide();

		if (child->IsVisible())
		{
			child->SetPosition(position);
			child->PrepareLayout();

			childSize = child->GetSize();
			position.y += childSize.y;

			UpdateSelfSize(child);
		}
	}
}

void CUI_TreeView::DeleteAllChildren()
{
	for (int i = myWidgets.Size() - 1; i >= 1; --i)
		myWidgets.DeleteCyclicAtIndex(i);
}

void CUI_TreeView::DeleteChildren(int aStartIndex)
{
	for (int i = myWidgets.Size() - 1; i >= 1 + aStartIndex; --i)
		myWidgets.DeleteCyclicAtIndex(i);
}

void CUI_TreeView::SetExpanded(bool aStatus)
{
	myIsExpanded = aStatus;
}

void CUI_TreeView::UpdateSelfSize(CUI_Widget* aChildWidget)
{
	CE_Vector2f posDif = aChildWidget->GetPosition() - GetPosition();
	CE_Vector2f childSize = aChildWidget->GetSize();
	childSize.x += posDif.x;
	

	if (childSize.x > mySize.x)
		mySize.x = childSize.x;

	mySize.y += childSize.y;
}

void CUI_TreeView::OnToggleViewClick()
{
	SetExpanded(!myIsExpanded);
}