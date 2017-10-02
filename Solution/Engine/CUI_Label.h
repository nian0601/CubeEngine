#pragma once
#include "CUI_Widget.h"

class CE_Font;
class CUI_Label : public CUI_Widget
{
public:
	CUI_Label(const CE_Font& aFont);
	CUI_Label(const CE_Font& aFont, const char* aString);
	CUI_Label(const CE_Font& aFont, const CE_String& aString);

	void Render(CE_RendererProxy& anRendererProxy) override;

	void SetText(const CE_String& aString);

private:
	const CE_Font& myFont;
	CE_String myString;
};

