#include "stdafx.h"

#include "CUI_Manager.h"
#include "CUI_Message.h"
#include "CUI_WIdget.h"

#include "CE_Input.h"

CUI_Manager::CUI_Manager()
{
	myDragMessage = new CUI_DragMessage();
}

CUI_Manager::~CUI_Manager()
{
	CE_SAFE_DELETE(myDragMessage);
}

void CUI_Manager::Update(const CE_Input& someInput)
{
	PrepareLayout();

	CE_Vector2f mousePos = someInput.GetMousePosition();
	CUI_MouseMessage mouseMessage;
	mouseMessage.myNewPosition = mousePos;
	mouseMessage.myOldPosition = myOldMousePosition;
	mouseMessage.myType = CUI_MouseMessage::MOUSE_MOVE;

	OnMouseMove(mouseMessage);

	if (someInput.MouseDown(0))
	{
		mouseMessage.myType = CUI_MouseMessage::MOUSE_DOWN;
		OnMouseDown(mouseMessage);

		myDragMessage->myType = CUI_DragMessage::DRAG_START;
		myDragMessage->myIntVar = -1;
		OnDragBegin(*myDragMessage);
	}
	else if (someInput.MouseUp(0))
	{
		if (myDragMessage->myIntVar != -1)
		{
			myDragMessage->myType = CUI_DragMessage::DRAG_END;
			OnDragEnd(*myDragMessage);
		}

		mouseMessage.myType = CUI_MouseMessage::MOUSE_UP;
		OnMouseUp(mouseMessage);
	}

	myOldMousePosition = mousePos;
}
