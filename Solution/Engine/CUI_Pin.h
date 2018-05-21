#pragma once
#include "CUI_Widget.h"

class CUI_Pin : public CUI_Widget
{
	friend class CUI_NodeEditor;

public:
	CUI_Pin(bool aIsInput, const CE_Vector2f& aSize, const CE_Vector4f& aColor);

	void Render(CE_RendererProxy& anRendererProxy) override;

	bool OnDragMessage(CUI_DragMessage& aMessage) override;

	bool IsInput() const { return myIsInput; }
	const CE_GrowingArray<CUI_Pin*>& GetConnections() const { return myConnections; }

	unsigned int myID;
private:
	CE_Vector4f myColor;
	bool myIsInput;

	CE_GrowingArray<CUI_Pin*> myConnections;
};

