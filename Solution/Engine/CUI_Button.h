#pragma once

#include "CUI_Container.h"
#include <functional>

class CE_Font;
class CUI_Button : public CUI_Container
{
public:
	CUI_Button(const CE_Vector2f& aSize, const CE_Vector4f& aColor);
	CUI_Button(const CE_String& aText);

	virtual void PrepareLayout() override;
	virtual bool OnClick() override;

	virtual bool OnMouseDown(const CUI_MouseMessage& aMessage) override;
	virtual bool OnMouseUp(const CUI_MouseMessage& aMessage) override;

	std::function<void()> myOnClick;

private:
	CUI_Widget* myWidget;
};

