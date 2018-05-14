#pragma once
#include "CUI_Container.h"

class CUI_Label;

class CE_Font;

class CUI_VisualNode : public CUI_Container
{
public:
	CUI_VisualNode(const CE_Font& aFont, const char* aLabel);
	~CUI_VisualNode();

	void PrepareLayout() override;
	void Render(CE_RendererProxy& anRendererProxy) override;

	bool OnMouseMessage(const CUI_MouseMessage& aMessage) override;

	bool CanBeFocused() const override { return true; }

	void AddPin(bool aIsInput);

private:
	CE_Vector4f myColor;
	CE_Vector2f myPositionOffset;

	float myPinSize;
	CUI_Label* myLabel;
};

