#pragma once

#include "CUI_Defines.h"
#include "CUI_VBox.h"

class CE_Font;
class CE_Input;
class CE_RendererProxy;
class CE_Texture;

class CUI_Widget;

struct CUI_DragMessage;

class CUI_Manager : public CUI_VBox
{
public:
	CUI_Manager();
	~CUI_Manager();

	void Update(const CE_Input& someInput);
	
	CE_Font& GetFont() const { return *myFont; }
	const CE_Texture* GetTexture(const char* aTextureName) const;

private:
	void SendMouseMessage(CUI_MouseMessage& aMessage, int aMouseButton, const CE_Input& someInput);

	CE_Vector2f myOldMousePosition;
	CUI_DragMessage* myDragMessage;

	CE_Font* myFont;
};

