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

	virtual void SetPosition(const CE_Vector2f& aPosition) { myPosition = aPosition; }
	virtual void SetSize(const CE_Vector2f& aSize) { mySize = aSize; }

protected:
	CUI_Widget() {};

	CE_Vector2f mySize;
	CE_Vector2f myPosition;
};

