#pragma once
#include "CUI_Label.h"

class CUI_EditBox : public CUI_Label
{
public:
	CUI_EditBox(float aWidth);
	~CUI_EditBox();

	virtual void PrepareLayout() override;

	void Render(CE_RendererProxy& anRendererProxy) override;
	bool OnTextInput(const CE_WindowMessage& aMessage) override;

	virtual bool CanBeFocused() const override { return true; }

private:
	float myWidth;
};

