#pragma once
#include "CUI_Widget.h"

class CE_Texture;
class CUI_Image : public CUI_Widget
{
public:
	CUI_Image(const char* aTexture);
	CUI_Image(const CE_Texture* aTexture);
	CUI_Image(const CE_Vector4f& aColor);
	CUI_Image(const CE_Vector2f& aSize);
	CUI_Image(const CE_Vector2f& aSize, const CE_Vector4f& aColor);
	CUI_Image(const CE_Vector2f& aSize, const CE_Vector4f& aColor, const CE_Vector2f& aHotspot);

	void SetColor(const CE_Vector4f& aColor) { myColor = aColor; }
	void SetTexture(const CE_Texture* aTexture) { myTexture = aTexture; }

	void Render(CE_RendererProxy& anRendererProxy) override;

protected:
	CE_Vector4f myColor;
	CE_Vector2f myHotspot;
	const CE_Texture* myTexture;
};

