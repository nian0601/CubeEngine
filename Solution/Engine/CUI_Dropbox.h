#pragma once
#include "CUI_Container.h"
#include <functional>

class CE_OldFont;
class CUI_Dropbox : public CUI_Container
{
public:
	CUI_Dropbox(const CE_String& aString);

	virtual void PrepareLayout() override;
	virtual void Render(CE_RendererProxy& anRendererProxy) override;

	virtual bool OnMouseUp(const CUI_MouseMessage& aMessage) override;
	virtual bool OnMouseWheel(const CUI_MouseMessage& aMessage) override;
	virtual bool OnMouseMessage(const CUI_MouseMessage& aMessage) override;

	virtual void DeleteAllChildren() override;

	void AddLabel(const char* aString);
	void SetExpansion(bool aStatus) { myIsExpanded = aStatus; }

	std::function<void(CUI_Widget*, int)> myOnSelection;

private:
	bool IsEmpty() const { return GetLabelCount() == 0; }
	void ExpandSize(const CE_Vector2f& aNewSize, CE_Vector2f& aSizeOut) const;
	void OnToggleExpansionClick();

	int GetLabelCount() const { return myWidgets.Size() - 2; }

	bool myIsExpanded;

	int myFirstListIndex;
	int myMaxVisibleListCount;
};

