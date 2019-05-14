#include "stdafx.h"

#include "CUI_Manager.h"
#include "CUI_Message.h"
#include "CUI_WIdget.h"

#include "CE_Input.h"
#include "CE_Font.h"

CUI_Manager::CUI_Manager(CE_GPUContext& aGPUContext)
{
	myDragMessage = new CUI_DragMessage();

	myFont = new CE_Font();
	myFont->LoadFromFile("Data/Font/Decent_Font.png", aGPUContext);
}

CUI_Manager::~CUI_Manager()
{
	CE_SAFE_DELETE(myFont);
	CE_SAFE_DELETE(myDragMessage);
}

void CUI_Manager::Update(const CE_Input& someInput)
{
	PrepareLayout();

	CE_Vector2f mousePos = someInput.GetMousePosition();
	CUI_MouseMessage mouseMessage;
	mouseMessage.myNewPosition = mousePos;
	mouseMessage.myOldPosition = myOldMousePosition;
	mouseMessage.myMouseWheelDelta = someInput.GetMouseDZ();
	mouseMessage.myModifiers = 0;

	if (someInput.KeyIsPressed(DIK_LSHIFT))
		mouseMessage.myModifiers |= CUI_MouseMessage::SHIFT;

	if (someInput.KeyIsPressed(DIK_LALT))
		mouseMessage.myModifiers |= CUI_MouseMessage::ALT;

	if (someInput.KeyIsPressed(DIK_LCONTROL))
		mouseMessage.myModifiers |= CUI_MouseMessage::CTRL;

	mouseMessage.myType = CUI_MouseMessage::MOUSE_MOVE;

	OnMouseMove(mouseMessage);

	SendMouseMessage(mouseMessage, 0, someInput);
	SendMouseMessage(mouseMessage, 1, someInput);
	SendMouseMessage(mouseMessage, 2, someInput);

	if (mouseMessage.myMouseWheelDelta != 0.f)
	{
		mouseMessage.myType = CUI_MouseMessage::MOUSE_WHEEL;
		OnMouseWheel(mouseMessage);
	}

	myOldMousePosition = mousePos;
}

void CUI_Manager::SendMouseMessage(CUI_MouseMessage& aMessage, int aMouseButton, const CE_Input& someInput)
{
	aMessage.myMouseButton = aMouseButton;

	if (someInput.MouseDown(aMouseButton))
	{
		aMessage.myType = CUI_MouseMessage::MOUSE_DOWN;
		OnMouseDown(aMessage);

		if (aMouseButton == 0)
		{
			myDragMessage->myType = CUI_DragMessage::DRAG_START;
			myDragMessage->myData.Clear();
			OnDragBegin(*myDragMessage);
		}
	}
	else if (someInput.MouseUp(aMouseButton))
	{
		if (aMouseButton == 0 && myDragMessage->myData.IsEmpty() == false)
		{
			myDragMessage->myType = CUI_DragMessage::DRAG_END;
			OnDragEnd(*myDragMessage);
		}

		aMessage.myType = CUI_MouseMessage::MOUSE_UP;
		OnMouseUp(aMessage);
	}
}
