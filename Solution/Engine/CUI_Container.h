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
	bool OnMouseWheel(const CUI_MouseMessage& aMessage) override;

	bool OnMouseMove(const CUI_MouseMessage& aMessage) override;
	bool OnTextInput(const CE_WindowMessage& aMessage);

	bool OnDragBegin(CUI_DragMessage& aMessage);
	bool OnDragEnd(CUI_DragMessage& aMessage);

	void AddWidget(CUI_Widget* aWidget) { myWidgets.Add(aWidget); aWidget->SetParent(this); }
	
	virtual void DeleteAllChildren();
	void DeleteWidget(CUI_Widget* aWidget);

	void Show() override;
	void Hide() override;

protected:
	CUI_Container() {};

	CE_GrowingArray<CUI_Widget*> myWidgets;
};

