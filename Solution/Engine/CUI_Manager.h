#pragma once

#include "CUI_VBox.h"

class CE_Input;
class CE_RendererProxy;

class CUI_Widget;
class CUI_Manager : public CUI_VBox
{
public:
	CUI_Manager();

	void Update(const CE_Input& someInput);
	
private:
	CE_Vector2f myOldMousePosition;
};

