#include "stdafx.h"
#include "CUI_HBox.h"

void CUI_HBox::PrepareLayout()
{
	if (myWidgets.Size() == 0)
		return;

	CE_Vector2f position = GetPosition();

	myWidgets[0]->SetPosition(position);
	myWidgets[0]->PrepareLayout();

	mySize = myWidgets[0]->GetSize();
	position.x += mySize.x;

	for (int i = 1; i < myWidgets.Size(); ++i)
	{
		CUI_Widget* child = myWidgets[i];
		child->SetPosition(position);
		child->PrepareLayout();

		CE_Vector2f childSize = child->GetSize();
		position.x += childSize.x;

		if (childSize.y > mySize.y)
			mySize.y = childSize.y;

		mySize.x += childSize.x;
	}
}
