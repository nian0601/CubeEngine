#pragma once
#include "CUI_Widget.h"

class CE_Font;
class CUI_ValueController;

class CUI_Label : public CUI_Widget
{
public:
	CUI_Label(const CE_Font& aFont);
	CUI_Label(const CE_Font& aFont, const char* aString);
	CUI_Label(const CE_Font& aFont, const CE_String& aString);
	CUI_Label(const CE_Font& aFont, CUI_ValueController* aValueController);
	~CUI_Label();

	void Render(CE_RendererProxy& anRendererProxy) override;
	void PrepareLayout() override;

	void SetText(const CE_String& aString);
	const CE_String& GetText() const { return myString; }

private:
	const CE_Font& myFont;
	CE_String myString;
	CUI_ValueController* myValueController;
	CE_Vector4f myColor;
};

