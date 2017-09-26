#pragma once

class CE_Renderer;

class CE_RendererProxy
{
public:
	CE_RendererProxy(CE_Renderer& aRenderer);

	void AddCubeData(const CE_Matrix44f& anOrientation, const CE_Vector3f& aScale, const CE_Vector4f& aColor);
	void AddSpriteData(const CE_Vector2f& aPosition, const CE_Vector2f& aSize, const CE_Vector4f& aColor, const CE_Vector2f& aHotspot = CE_Vector2f(0.f, 0.f));

private:
	CE_Renderer& myRenderer;
};

