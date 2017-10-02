#pragma once
#include "CUI_Widget.h"

class CUI_Container : public CUI_Widget
{
public:
	virtual ~CUI_Container() override;

	virtual void Render(CE_RendererProxy& anRendererProxy) override;
	virtual bool OnClick(const CE_Vector2f& aMousePosition) override;

	void AddWidget(CUI_Widget* aWidget) { myWidgets.Add(aWidget); }

protected:
	CUI_Container() {};

	CE_GrowingArray<CUI_Widget*> myWidgets;
};

