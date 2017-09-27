#pragma once
class CE_RendererProxy;
class CUI_Widget
{
public:
	virtual ~CUI_Widget() {};

	virtual void PrepareLayout() {};
	virtual void FinalizeLayout();
	virtual void Render(CE_RendererProxy& anRendererProxy) = 0;

	virtual const CE_Vector2f& GetPosition() const { return myPosition; }
	virtual const CE_Vector2f& GetScreenPosition() const { return myScreenPosition; }
	virtual const CE_Vector2f& GetSize() const { return mySize; }
	virtual const CE_Vector2f& GetScreenSize() const { return myScreenSize; }

	virtual void SetPosition(const CE_Vector2f& aPosition);
	virtual void SetSize(const CE_Vector2f& aSize);
	virtual void SetWindowSize(const CE_Vector2f& aSize);

	virtual bool OnClick(const CE_Vector2f& aMousePosition);

protected:
	virtual void UpdateScreenPosition();
	virtual void UpdateScreenSize();

	CUI_Widget() {};

	CE_Vector2f myPosition;
	CE_Vector2f mySize;

	CE_Vector2f myScreenPosition;
	CE_Vector2f myScreenSize;

	CE_Vector2f myWindowSize;
};