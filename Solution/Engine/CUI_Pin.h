#pragma once
#include "CUI_Widget.h"

class CUI_Pin : public CUI_Widget
{
public:
	CUI_Pin(bool aIsInput, const CE_Vector2f& aSize, const CE_Vector4f& aColor);

	void Render(CE_RendererProxy& anRendererProxy) override;

	bool OnDragMessage(CUI_DragMessage& aMessage) override;
	bool OnMouseMessage(const CUI_MouseMessage& aMessage) override;

	bool IsInput() const { return myIsInput; }

	void OnLostFocus() override;

	const CE_GrowingArray<CUI_Pin*>& GetConnections() const { return myConnections; }

private:
	void RenderSteppedLine(CE_RendererProxy& anRendererProxy, const CE_Vector2f& aStartPos, const CE_Vector2f& aEndPos, float aCutPoint);
	CE_Vector4f myColor;
	bool myIsInput;

	CE_Vector2f myMousePosition;
	bool myHasStartedDrag;

	CE_GrowingArray<CUI_Pin*> myConnections;
};

