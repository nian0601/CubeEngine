#pragma once
#include "CUI_Container.h"
#include <functional>

class CE_Font;
class CUI_Dropbox : public CUI_Container
{
public:
	CUI_Dropbox(const CE_Font& aFont, const CE_String& aString);

	virtual void PrepareLayout() override;
	virtual void Render(CE_RendererProxy& anRendererProxy) override;

	virtual bool OnMouseUp(const CE_Vector2f& aMousePosition) override;

	virtual void DeleteAllChildren() override;

	void AddLabel(const char* aString);

	std::function<void(CUI_Widget*)> myOnSelection;

private:
	bool IsEmpty() const { return myWidgets.Size() <= 2; }
	void ExpandSize(const CE_Vector2f& aNewSize, CE_Vector2f& aSizeOut) const;
	void OnToggleExpansionClick();

	bool myIsExpanded;
	const CE_Font& myFont;
};

