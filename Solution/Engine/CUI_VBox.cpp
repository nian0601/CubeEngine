#include "stdafx.h"
#include "CUI_VBox.h"


void CUI_VBox::PrepareLayout()
{
	if (myWidgets.Size() == 0)
		return;

	CE_Vector2f position = GetPosition();

	myWidgets[0]->SetPosition(position);
	myWidgets[0]->PrepareLayout();

	mySize = myWidgets[0]->GetSize();
	position.y += mySize.y;

	for (int i = 1; i < myWidgets.Size(); ++i)
	{
		CUI_Widget* child = myWidgets[i];
		child->SetPosition(position);
		child->PrepareLayout();

		CE_Vector2f childSize = child->GetSize();
		position.x += childSize.x;

		if (childSize.x > mySize.x)
			mySize.x = childSize.x;

		mySize.y += childSize.y;
	}
}
