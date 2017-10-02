#include "stdafx.h"
#include "CUI_VBox.h"


void CUI_VBox::PrepareLayout()
{
	if (myWidgets.Size() == 0)
		return;

	CE_Vector2f position = GetPosition();
	mySize = CE_Vector2f(0.f);

	for (int i = 0; i < myWidgets.Size(); ++i)
	{
		CUI_Widget* child = myWidgets[i];
		child->SetPosition(position);
		child->PrepareLayout();

		CE_Vector2f childSize = child->GetSize();
		position.y += childSize.y;

		if (childSize.x > mySize.x)
			mySize.x = childSize.x;

		mySize.y += childSize.y;
	}
}
