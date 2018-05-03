#pragma once
#include "CUI_Widget.h"

class CUI_Image : public CUI_Widget
{
public:
	CUI_Image(const CE_Vector4f& aColor);
	CUI_Image(const CE_Vector2f& aSize);
	CUI_Image(const CE_Vector2f& aSize, const CE_Vector4f& aColor);
	CUI_Image(const CE_Vector2f& aSize, const CE_Vector4f& aColor, const CE_Vector2f& aHotspot);

	void Render(CE_RendererProxy& anRendererProxy) override;

private:
	CE_Vector4f myColor;
	CE_Vector2f myHotspot;
};

