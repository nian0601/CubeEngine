#pragma once

#include "CUI_Image.h"
#include <functional>

class CUI_Button : public CUI_Image
{
public:
	CUI_Button(const CE_Vector2f& aSize, const CE_Vector4f& aColor);

	virtual bool OnClick(const CE_Vector2f& aMousePosition) override;

	std::function<void()> myOnClick;
};

