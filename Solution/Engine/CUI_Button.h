#pragma once

#include <functional>

#include "CUI_Widget.h"

class CUI_Image;
class CUI_Label;

class CE_Font;
class CE_Texture;

class CUI_Button : public CUI_Widget
{
public:
	CUI_Button(const CE_String& aText);

	virtual void PrepareLayout() override;
	virtual void Render(CE_RendererProxy& anRendererProxy) override;

	virtual bool OnClick() override;

	std::function<void()> myOnClick;

private:
	CUI_Widget* myWidget;

	CUI_Image* myImage;
	CUI_Label* myLabel;

	const CE_Texture* myNormalTexture;
	const CE_Texture* myHoverTexture;
	const CE_Texture* myPressTexture;
};

