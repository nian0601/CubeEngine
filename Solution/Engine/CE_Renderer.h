#pragma once

class CE_Camera;
class CE_GPUContext;

class CE_Text;
class CE_LineRenderObject;
class CE_RendererProxy;

class CE_RenderObject;
class CE_ConstantBuffer;

class CE_ShaderPair;
class CE_ShaderManager;

#include "CE_DebugPrimitives.h"
#include "CE_RendererProxy.h"

class CE_Renderer
{
public:
	CE_Renderer(CE_GPUContext& anGPUContext, CE_ShaderManager& aShaderManager);
	~CE_Renderer();

	void UpdateConstantBuffers(const CE_Camera& aCamera);
	void Render3D(const CE_RendererProxy& aRendererProxy);
	void Render2D(const CE_RendererProxy& aRendererProxy);
	void RenderLines(const CE_GrowingArray<CE_Line>& someLines);

private:
	void RenderModels(const CE_RendererProxy& aRendererProxy);
	void RenderText(const CE_2DData& aTextData);
	void RenderSprite(const CE_2DData& aSpriteData);
	void Render2DLine(const CE_2DData& aLineData);

	CE_GPUContext& myGPUContext;

	CE_RenderObject* myCubeModel;
	CE_RenderObject* mySphereModel;

	CE_RenderObject* mySprite;
	CE_ShaderPair* mySpriteShader;

	CE_Text* myText;
	CE_ShaderPair* myTextShader;

	CE_LineRenderObject* myLineObject;

	CE_ConstantBuffer* myViewProjectionConstantBuffer;
	CE_ConstantBuffer* myOrthagonalConstantBuffer;

	CE_ConstantBuffer* myModelObjectDataConstantBuffer;

	CE_ShaderPair* myCubeShader;
	CE_ShaderPair* myLineShader;
	CE_ShaderPair* myLine2DShader;
};

