#pragma once

struct CE_WindowMessage;

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

	virtual void OnMouseDown(const CE_Vector2f& aMousePosition);
	virtual bool OnMouseUp(const CE_Vector2f& aMousePosition);
	virtual void OnMouseEnter() { myIsHovered = true; };
	virtual void OnMouseExit() { myIsHovered = false; };

	virtual void OnMouseMove(const CE_Vector2f& aNewMousePosition, const CE_Vector2f& aOldMousePosition);
	virtual bool OnTextInput(const CE_WindowMessage& aMessage) { aMessage; return false; }

	void Show() { myIsVisible = true; }
	void Hide() { myIsVisible = false; myHasLongPress = false; myIsHovered = false; myIsFocused = false; }
	bool IsVisible() const { return myIsVisible; }
	bool IsFocused() const { return myIsFocused; }

	virtual bool CanBeFocused() const { return false; }

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