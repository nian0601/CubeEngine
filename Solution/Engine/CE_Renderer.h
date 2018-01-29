#pragma once

class CE_Camera;
class CE_GPUContext;
class CE_Model;
class CE_CubeShader;

class CE_Sprite;
class CE_SpriteShader;

class CE_Text;
class CE_TextShader;

class CE_LineRenderObject;

class CE_RendererProxy;

#include "CE_DebugPrimitives.h"

class CE_Renderer
{
public:
	CE_Renderer(CE_GPUContext& anGPUContext);
	~CE_Renderer();

	void Render3D(CE_Camera& aCamera, const CE_RendererProxy& aRendererProxy);
	void Render2D(const CE_Matrix44f& aOrthagonalMatrix, const CE_RendererProxy& aRendererProxy);
	void RenderLines(CE_Camera& aCamera, const CE_GrowingArray<CE_Line>& someLines);

private:
	void RenderCubes(CE_Camera& aCamera, const CE_RendererProxy& aRendererProxy);
	void RenderSprites(const CE_Matrix44f& aOrthagonalMatrix, const CE_RendererProxy& aRendererProxy);
	void RenderTexts(const CE_Matrix44f& aOrthagonalMatrix, const CE_RendererProxy& aRendererProxy);

	CE_GPUContext& myGPUContext;

	CE_CubeShader* myCubeShader;
	CE_Model* myCubeModel;

	CE_Sprite* mySprite;
	CE_SpriteShader* mySpriteShader;

	CE_Text* myText;
	CE_TextShader* myTextShader;

	CE_Text* myMSDFText;
	CE_TextShader* myMSDFTextShader;

	CE_LineRenderObject* myLineObject;
	CE_CubeShader* myLineShader;
};

