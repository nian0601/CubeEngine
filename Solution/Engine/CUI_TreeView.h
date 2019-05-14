#pragma once

#include "CUI_VBox.h"

class CUI_Button;
class CE_Font;
class CUI_TreeView : public CUI_VBox
{
public:
	CUI_TreeView();
	CUI_TreeView(const CE_String& aString);

	void PrepareLayout();

	void DeleteAllChildren() override;
	void DeleteChildren(int aStartIndex);
	void SetExpanded(bool aStatus);

private:
	void UpdateSelfSize(CUI_Widget* aChildWidget);
	void OnToggleViewClick();

	CUI_Button* myButton;
	bool myIsExpanded;
};

