#pragma once

class CE_Renderer;

class CE_RendererProxy
{
public:
	CE_RendererProxy(CE_Renderer& aRenderer);

	void AddCubeData(const CE_Matrix44f& anOrientation, const CE_Vector3f& aScale, const CE_Vector4f& aColor);

private:
	CE_Renderer& myRenderer;
};

