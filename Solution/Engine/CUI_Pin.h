#pragma once
#include "CUI_Widget.h"

class CUI_Pin : public CUI_Widget
{
public:
	CUI_Pin(bool aIsInput, const CE_Vector2f& aSize, const CE_Vector4f& aColor);

	void Render(CE_RendererProxy& anRendererProxy) override;
	void OnMouseMove(const CE_Vector2f& aNewMousePosition, const CE_Vector2f& aOldMousePosition) override;

	bool IsInput() const { return myIsInput; }
private:
	CE_Vector4f myColor;
	bool myIsInput;
};

