#pragma once

#include "CUI_VBox.h"

class CE_Input;
class CE_RendererProxy;

class CUI_Widget;

struct CUI_DragMessage;

class CUI_Manager : public CUI_VBox
{
public:
	CUI_Manager();
	~CUI_Manager();

	void Update(const CE_Input& someInput);
	
private:
	void SendMouseMessage(CUI_MouseMessage& aMessage, int aMouseButton, const CE_Input& someInput);

	CE_Vector2f myOldMousePosition;
	CUI_DragMessage* myDragMessage;
};

