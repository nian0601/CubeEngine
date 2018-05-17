#pragma once

struct CE_WindowMessage;

struct CUI_DragMessage;
struct CUI_MouseMessage;

class CE_RendererProxy;

class CUI_Widget
{
public:
	virtual ~CUI_Widget() {};

	virtual void PrepareLayout() {};
	virtual void Render(CE_RendererProxy& anRendererProxy) = 0;

	virtual const CE_Vector2f& GetPosition() const { return myPosition; }
	virtual const CE_Vector2f& GetSize() const { return mySize; }

	virtual void SetPosition(const CE_Vector2f& aPosition);
	virtual void SetSize(const CE_Vector2f& aSize);

	virtual bool OnClick() { return false; }

	virtual bool OnMouseDown(const CUI_MouseMessage& aMessage);
	virtual bool OnMouseUp(const CUI_MouseMessage& aMessage);
	virtual void OnMouseEnter() { myIsHovered = true; };
	virtual void OnMouseExit() { myIsHovered = false; };
	virtual bool OnMouseMove(const CUI_MouseMessage& aMessage);

	virtual bool OnDragBegin(CUI_DragMessage& aMessage);
	virtual bool OnDragEnd(CUI_DragMessage& aMessage);

	virtual bool OnMouseMessage(const CUI_MouseMessage& aMessage) { aMessage; return false; }
	virtual bool OnTextInput(const CE_WindowMessage& aMessage) { aMessage; return false; }
	virtual bool OnDragMessage(CUI_DragMessage& aMessage) { aMessage; return false; }

	virtual void OnGainedFocus() {}
	virtual void OnLostFocus() {}
	virtual bool CanBeFocused() const { return false; }

	void Show() { myIsVisible = true; }
	void Hide() { myIsVisible = false; myHasLongPress = false; myIsHovered = false; myIsFocused = false; }
	bool IsVisible() const { return myIsVisible; }
	bool IsFocused() const { return myIsFocused; }

	bool Contains(const CE_Vector2f& aPosition) const;

	bool OnWindowMessage(const CE_WindowMessage& aMessage);

protected:
	CUI_Widget() 
		: myIsVisible(true)
		, myHasLongPress(false)
		, myIsHovered(false)
		, myIsFocused(false)
	{};

	CE_Vector2f myPosition;
	CE_Vector2f mySize;

	CE_Vector2f myWindowSize;

	bool myIsVisible;
	bool myHasLongPress;
	bool myIsHovered;
	bool myIsFocused;
};