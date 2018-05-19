#pragma once
#include "CUI_Container.h"

class CUI_Label;
class CUI_Pin;

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
	void RenderConnections(CE_RendererProxy& anRendererProxy);
	void RenderSteppedLine(CE_RendererProxy& anRendererProxy, const CE_Vector2f& aStartPos, const CE_Vector2f& aEndPos, float aCutPoint);

	CE_Vector4f myColor;
	CE_Vector2f myPositionOffset;

	float myPinSize;
	CUI_Label* myLabel;

	CE_GrowingArray<CUI_Pin*> myInputs;
	CE_GrowingArray<CUI_Pin*> myOutPuts;
};

