#include "stdafx.h"
#include "CUI_Manager.h"
#include "CUI_WIdget.h"
#include "CE_Input.h"

CUI_Manager::CUI_Manager(CE_Input& anInput)
	: myInput(anInput)
{
	myWindowSize.x = 1280.f;
	myWindowSize.y = 720.f;
}

void CUI_Manager::Update()
{
	PrepareLayout();

	if (myInput.MouseDown(0))
	{
		CE_Vector2f mousePos = myInput.GetMousePosition();
		OnClick(mousePos);
	}
}
