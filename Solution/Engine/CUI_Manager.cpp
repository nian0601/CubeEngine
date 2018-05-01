#include "stdafx.h"
#include "CUI_Manager.h"
#include "CUI_WIdget.h"
#include "CE_Input.h"

CUI_Manager::CUI_Manager(CE_Input& anInput)
	: myInput(anInput)
{
}

void CUI_Manager::Update()
{
	PrepareLayout();

	CE_Vector2f mousePos = myInput.GetMousePosition();
	OnMouseMove(mousePos, myOldMousePosition);

	if (myInput.MouseDown(0))
		OnMouseDown(mousePos);
	else if (myInput.MouseUp(0))
		OnMouseUp(mousePos);

	myOldMousePosition = mousePos;
}
