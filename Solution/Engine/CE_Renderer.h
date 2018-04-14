#pragma once

class CE_Camera;
class CE_GPUContext;

class CE_Text;
class CE_LineRenderObject;
class CE_RendererProxy;

class CE_RenderObject;
class CE_ConstantBuffer;

class CE_ShaderPair;

#include "CE_DebugPrimitives.h"

class CE_Renderer
{
public:
	CE_Renderer(CE_GPUContext& anGPUContext);
	~CE_Renderer();

	void UpdateConstantBuffers(const CE_Camera& aCamera);
	void Render3D(const CE_RendererProxy& aRendererProxy);
	void Render2D(const CE_RendererProxy& aRendererProxy);
	void RenderLines(const CE_GrowingArray<CE_Line>& someLines);

private:
	void RenderModels(const CE_RendererProxy& aRendererProxy);
	void RenderSprites(const CE_RendererProxy& aRendererProxy);
	void RenderTexts(const CE_RendererProxy& aRendererProxy);

	CE_GPUContext& myGPUContext;

	CE_RenderObject* myCubeModel;
	CE_RenderObject* mySphereModel;

	CE_RenderObject* mySprite;
	CE_ShaderPair* mySpriteShader;

	CE_Text* myText;
	CE_ShaderPair* myTextShader;

	CE_Text* myMSDFText;
	CE_ShaderPair* myMSDFTextShader;

	CE_LineRenderObject* myLineObject;

	CE_ConstantBuffer* myViewProjectionConstantBuffer;
	CE_ConstantBuffer* myOrthagonalConstantBuffer;

	CE_ConstantBuffer* myModelObjectDataConstantBuffer;

	CE_ShaderPair* myCubeShader;
	CE_ShaderPair* myLineShader;

};

