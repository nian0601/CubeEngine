#include "stdafx.h"
#include "CUI_Manager.h"
#include "CUI_WIdget.h"
#include "CE_Input.h"

CUI_Manager::CUI_Manager()
{
}

void CUI_Manager::Update(const CE_Input& someInput)
{
	PrepareLayout();

	CE_Vector2f mousePos = someInput.GetMousePosition();
	OnMouseMove(mousePos, myOldMousePosition);

	if (someInput.MouseDown(0))
		OnMouseDown(mousePos);
	else if (someInput.MouseUp(0))
		OnMouseUp(mousePos);

	myOldMousePosition = mousePos;
}
