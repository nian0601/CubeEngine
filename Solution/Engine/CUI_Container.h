#pragma once
#include "CUI_Widget.h"

class CUI_Container : public CUI_Widget
{
public:
	virtual ~CUI_Container() override;

	virtual void Render(CE_RendererProxy& anRendererProxy) override;
	virtual void OnMouseDown(const CE_Vector2f& aMousePosition) override;
	virtual bool OnMouseUp(const CE_Vector2f& aMousePosition) override;
	virtual void OnMouseMove(const CE_Vector2f& aNewMousePosition, const CE_Vector2f& aOldMousePosition) override;
	virtual bool OnTextInput(const CE_WindowMessage& aMessage);

	void AddWidget(CUI_Widget* aWidget) { myWidgets.Add(aWidget); }
	
	virtual void DeleteAllChildren();

protected:
	CUI_Container() {};

	CE_GrowingArray<CUI_Widget*> myWidgets;
};

