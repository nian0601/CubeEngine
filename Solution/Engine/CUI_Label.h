#pragma once
#include "CUI_Widget.h"

class CUI_ValueController;

class CUI_Label : public CUI_Widget
{
public:
	CUI_Label();
	CUI_Label(const char* aString);
	CUI_Label(const CE_String& aString);
	CUI_Label(CUI_ValueController* aValueController);
	~CUI_Label();

	void Render(CE_RendererProxy& anRendererProxy) override;
	void PrepareLayout() override;

	bool OnMouseMessage(const CUI_MouseMessage& aMessage) override;

	void SetText(const CE_String& aString);
	const CE_String& GetText() const { return myString; }
	void SetColor(const CE_Vector4f& aColor) { myColor = aColor; }

protected:
	CE_String myString;
	CUI_ValueController* myValueController;
	CE_Vector4f myColor;
};

