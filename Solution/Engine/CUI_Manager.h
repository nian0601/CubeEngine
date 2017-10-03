#pragma once

#include "CUI_VBox.h"

class CE_Input;
class CE_RendererProxy;

class CUI_Widget;
class CUI_Manager : public CUI_VBox
{
public:
	CUI_Manager(CE_Input& anInput);

	void Update();

private:
	CE_Input& myInput;
	CE_Vector2f myWindowSize;
};

