#pragma once
#include "CUI_Widget.h"

class CUI_Container : public CUI_Widget
{
public:
	virtual ~CUI_Container() override;

	virtual void PrepareLayout() override;
	virtual void Render(CE_RendererProxy& anRendererProxy) override;

	bool OnMouseDown(const CUI_MouseMessage& aMessage) override;
	bool OnMouseUp(const CUI_MouseMessage& aMessage) override;

	bool OnMouseMove(const CUI_MouseMessage& aMessage) override;
	bool OnTextInput(const CE_WindowMessage& aMessage);

	bool OnDragBegin(CUI_DragMessage& aMessage);
	bool OnDragEnd(CUI_DragMessage& aMessage);

	void AddWidget(CUI_Widget* aWidget) { myWidgets.Add(aWidget); }
	
	virtual void DeleteAllChildren();

protected:
	CUI_Container() {};

	CE_GrowingArray<CUI_Widget*> myWidgets;
};

