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

	virtual bool OnClick(const CE_Vector2f& aMousePosition);

	void Show() { myIsVisible = true; }
	void Hide() { myIsVisible = false; }
	bool IsVisible() const { return myIsVisible; }

protected:
	CUI_Widget() : myIsVisible(true) {};

	CE_Vector2f myPosition;
	CE_Vector2f mySize;

	CE_Vector2f myWindowSize;

	bool myIsVisible;
};