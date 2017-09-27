#pragma once

class CE_Input;
class CE_RendererProxy;

class CUI_Widget;
class CUI_Manager
{
public:
	CUI_Manager(CE_Input& anInput);
	~CUI_Manager();

	void AddWidget(CUI_Widget* anWidget);

	void Update();
	void Render(CE_RendererProxy& anRendererProxy);

private:
	CE_Input& myInput;
	CE_GrowingArray<CUI_Widget*> myWidgets;
	CE_Vector2f myWindowSize;
};

