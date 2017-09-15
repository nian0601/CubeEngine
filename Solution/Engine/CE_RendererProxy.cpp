#include "stdafx.h"
#include "CE_RendererProxy.h"
#include "CE_Renderer.h"


CE_RendererProxy::CE_RendererProxy(CE_Renderer& aRenderer)
	: myRenderer(aRenderer)
{
}

void CE_RendererProxy::AddCubeData(const CE_Matrix44f& anOrientation, const CE_Vector4f& aColor)
{
	myRenderer.AddCubeData(anOrientation, aColor);
}
