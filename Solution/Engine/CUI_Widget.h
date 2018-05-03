#pragma once
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

	virtual bool OnClick() { return false; };
	virtual void OnMouseDown(const CE_Vector2f& aMousePosition);
	virtual bool OnMouseUp(const CE_Vector2f& aMousePosition);
	virtual void OnMouseEnter() { myIsHovered = true; };
	virtual void OnMouseExit() { myIsHovered = false; };

	virtual void OnMouseMove(const CE_Vector2f& aNewMousePosition, const CE_Vector2f& aOldMousePosition);

	void Show() { myIsVisible = true; }
	void Hide() { myIsVisible = false; myIsFocused = false; myIsHovered = false; }
	bool IsVisible() const { return myIsVisible; }

	bool Contains(const CE_Vector2f& aPosition) const;

protected:
	CUI_Widget() 
		: myIsVisible(true)
		, myIsFocused(false)
		, myIsHovered(false)
	{};

	CE_Vector2f myPosition;
	CE_Vector2f mySize;

	CE_Vector2f myWindowSize;

	bool myIsVisible;
	bool myIsFocused;
	bool myIsHovered;
};