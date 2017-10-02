#pragma once

#include "CUI_VBox.h"

class CUI_Button;
class CUI_TreeView : public CUI_VBox
{
public:
	CUI_TreeView();

private:
	void OnToggleViewClick();

	CUI_Button* myButton;
	bool myIsExpanded;
};

