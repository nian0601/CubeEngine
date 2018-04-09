#pragma once

class CE_Camera;
class CE_GPUContext;

class CE_Text;
class CE_LineRenderObject;
class CE_RendererProxy;
class CE_Shader;

class CE_RenderObject;

#include "CE_DebugPrimitives.h"

class CE_Renderer
{
public:
	CE_Renderer(CE_GPUContext& anGPUContext);
	~CE_Renderer();

	void Render3D(const CE_Camera& aCamera, const CE_RendererProxy& aRendererProxy);
	void Render2D(const CE_Matrix44f& aOrthagonalMatrix, const CE_RendererProxy& aRendererProxy);
	void RenderLines(const CE_Camera& aCamera, const CE_GrowingArray<CE_Line>& someLines);

private:
	void RenderModels(const CE_Camera& aCamera, const CE_RendererProxy& aRendererProxy);
	void RenderSprites(const CE_Matrix44f& aOrthagonalMatrix, const CE_RendererProxy& aRendererProxy);
	void RenderTexts(const CE_Matrix44f& aOrthagonalMatrix, const CE_RendererProxy& aRendererProxy);

	CE_GPUContext& myGPUContext;

	CE_Shader* myModelShader;
	CE_RenderObject* myCubeModel;
	CE_RenderObject* mySphereModel;

	CE_RenderObject* mySprite;
	CE_Shader* mySpriteShader;

	CE_Text* myText;
	CE_Shader* myTextShader;

	CE_Text* myMSDFText;
	CE_Shader* myMSDFTextShader;

	CE_LineRenderObject* myLineObject;
	CE_Shader* myLineShader;
};

