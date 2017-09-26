#pragma once

class CE_RendererProxy;

class CUI_Widget;
class CUI_Manager
{
public:
	~CUI_Manager();

	void AddWidget(CUI_Widget* anWidget);

	void Render(CE_RendererProxy& anRendererProxy);

private:
	CE_GrowingArray<CUI_Widget*> myWidgets;
};

