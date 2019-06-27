#include "stdafx.h"
#include "CE_Renderer.h"
#include "CE_DirextXFactory.h"
#include "CE_RendererProxy.h"
#include "CE_LineRenderObject.h"
#include "CE_Camera.h"
#include "CE_RenderObject.h"
#include "CE_ShaderStructs.h"
#include "CE_ShaderPair.h"
#include "CE_ShaderManager.h"
#include "CE_ObjLoader.h"
#include "CE_ObjManager.h"
#include "CE_MaterialManager.h"
#include "CE_GPUContext.h"
#include "CE_Texture.h"
#include "CE_TextureManager.h"
#include <d3d11.h>
#include "CE_GPUBuffer.h"
#include "CE_Font.h"

CE_Renderer::CE_Renderer(CE_GPUContext& anGPUContext)
	: myGPUContext(anGPUContext)
{
	SetupRenderObjects();
	SetupShaders();
	SetupConstantBuffers();

	myEmptyTexture = CE_TextureManager::GetInstance()->GetEmptyTexture();
	myFont = new CE_Font("c:/Windows/Fonts/arial.ttf", 32.f);
}

CE_Renderer::~CE_Renderer()
{
	CE_SAFE_DELETE(myModelObjectDataConstantBuffer);
	CE_SAFE_DELETE(myOrthagonalConstantBuffer);
	CE_SAFE_DELETE(myViewProjectionConstantBuffer);

	CE_SAFE_DELETE(myQuad);
	CE_SAFE_DELETE(mySphereModel);

	CE_SAFE_DELETE(myCubeModel);

	CE_SAFE_DELETE(myLineObject);

	CE_SAFE_DELETE(myLineShader);
	CE_SAFE_DELETE(myLine2DShader);
	CE_SAFE_DELETE(mySpriteShader);

	CE_SAFE_DELETE(myCubeShader);

	CE_SAFE_DELETE(myFont);
}

void CE_Renderer::UpdateConstantBuffers(const CE_Camera& aCamera)
{
	CE_ViewProjectionData viewProjection;
	viewProjection.myView = aCamera.GetView();
	viewProjection.myProjection = aCamera.GetProjection();
	myViewProjectionConstantBuffer->SetData(&viewProjection, sizeof(viewProjection));

	CE_ProjectionData orthagonal;
	orthagonal.myProjection = aCamera.GetOrthagonalProjection();
	myOrthagonalConstantBuffer->SetData(&orthagonal, sizeof(orthagonal));

	mySceeenSize.x = static_cast<float>(aCamera.GetWindowSize().x);
	mySceeenSize.y = static_cast<float>(aCamera.GetWindowSize().y);
}

void CE_Renderer::Render3D(const CE_RendererProxy& aRendererProxy)
{
	RenderModels(aRendererProxy);
	RenderObjs(aRendererProxy);
}

void CE_Renderer::Render2D(const CE_RendererProxy& aRendererProxy)
{
	myOrthagonalConstantBuffer->SendToGPU();

	for (const CE_2DData& data : aRendererProxy.Get2DData())
	{
		if (data.myType == CE_2DData::SPRITE)
			RenderQuad(data);
		else if (data.myType == CE_2DData::TEXT)
			RenderText(data);
		else if (data.myType == CE_2DData::LINE)
			Render2DLine(data);
		else
			CE_ASSERT_ALWAYS("Invalid 2D-data type!");
	}
}

void CE_Renderer::RenderLines(const CE_GrowingArray<CE_Line>& someLines)
{
	if (someLines.Size() == 0)
		return;

	CE_SetResetBlend blend(ALPHA_BLEND);
	CE_SetResetDepth depth(ENABLED);

	myViewProjectionConstantBuffer->SendToGPU();
	myLineShader->Activate();
	
	myLineObject->SetLines(someLines, myGPUContext);
	myLineObject->Render(myGPUContext);
}

void CE_Renderer::SetupRenderObjects()
{
	myCubeModel = new CE_RenderObject();
	myCubeModel->InitCube();

	mySphereModel = new CE_RenderObject();
	mySphereModel->InitSphere();

	myQuad = new CE_RenderObject();
	myQuad->InitSprite();
	myQuad->CreateObjectData(1, sizeof(CE_SpriteShaderData));

	myLineObject = new CE_LineRenderObject();
}

void CE_Renderer::SetupShaders()
{
	CE_ShaderManager* shaderManager = CE_ShaderManager::GetInstance();
	CE_GenericShader* cubeVX = shaderManager->GetShader("Cube.vx");
	CE_GenericShader* cubePX = shaderManager->GetShader("Cube.px");
	myCubeShader = new CE_ShaderPair(cubeVX, cubePX);

	CE_GenericShader* quadVX = shaderManager->GetShader("Quad.vx");
	mySpriteShader = new CE_ShaderPair(quadVX, shaderManager->GetShader("Sprite.px"));
	myTextShader = new CE_ShaderPair(quadVX, shaderManager->GetShader("Text.px"));

	CE_GenericShader* linePX = shaderManager->GetShader("Line.px");
	myLineShader = new CE_ShaderPair(shaderManager->GetShader("Line.vx"), linePX);
	myLine2DShader = new CE_ShaderPair(shaderManager->GetShader("Line2D.vx"), linePX);
}

void CE_Renderer::SetupConstantBuffers()
{
	myViewProjectionConstantBuffer = new CE_ConstantBuffer();
	myViewProjectionConstantBuffer->Init(0, sizeof(CE_ViewProjectionData));

	myModelObjectDataConstantBuffer = new CE_ConstantBuffer();
	myModelObjectDataConstantBuffer->Init(1, sizeof(CE_ModelShaderData));

	myOrthagonalConstantBuffer = new CE_ConstantBuffer();
	myOrthagonalConstantBuffer->Init(0, sizeof(CE_ProjectionData));
}

void CE_Renderer::RenderModels(const CE_RendererProxy& aRendererProxy)
{
	CE_SetResetRasterizer raster(CULL_BACK);
	CE_SetResetDepth depth(ENABLED);
	CE_SetResetBlend blend(NO_BLEND);

	myViewProjectionConstantBuffer->SendToGPU();
	myCubeShader->Activate();

	for (const CE_ModelData& data : aRendererProxy.GetModelData())
	{
		CE_RenderObject* model = myCubeModel;
		if (data.myIsSphere)
			model = mySphereModel;

		CE_ModelShaderData modelData;
		modelData.myWorld = data.myOrientation;
		modelData.myColorAndMetalness = data.myColorAndMetalness;
		modelData.myScaleAndRoughness = data.myScaleAndRoughness;

		myModelObjectDataConstantBuffer->SetData(&modelData, sizeof(modelData));
		myModelObjectDataConstantBuffer->SendToGPU();

		model->Render();
	}
}

void CE_Renderer::RenderObjs(const CE_RendererProxy& aRendererProxy)
{
	CE_SetResetRasterizer raster(CULL_BACK);
	CE_SetResetDepth depth(ENABLED);
	CE_SetResetBlend blend(NO_BLEND);

	myViewProjectionConstantBuffer->SendToGPU();
	myCubeShader->Activate();

	for (const CE_ObjRenderData& data : aRendererProxy.GetObjRenderData())
	{
		CE_ModelShaderData modelData;
		modelData.myWorld = data.myOrientation;

		const CE_ObjData* obj = CE_ObjManager::GetInstance()->GetObjData(data.myObjID);
		for (const CE_ObjMesh& mesh : obj->myMeshes)
		{	
			const CE_Material& material = *mesh.myMaterial;
			modelData.myColorAndMetalness = material.myDiffuse;
			modelData.myColorAndMetalness.w = material.myMetalness;
			modelData.myScaleAndRoughness.x = data.myScale.x;
			modelData.myScaleAndRoughness.y = data.myScale.y;
			modelData.myScaleAndRoughness.z = data.myScale.z;
			modelData.myScaleAndRoughness.w = material.myRoughness;

			myModelObjectDataConstantBuffer->SetData(&modelData, sizeof(modelData));
			myModelObjectDataConstantBuffer->SendToGPU();

			mesh.myRenderObject->Render();
		}
		
	}
}

void CE_Renderer::RenderText(const CE_2DData& aTextData)
{
	CE_2DData quadData;
	quadData.myType = CE_2DData::TEXT;

	CE_Vector2f currentPoint = aTextData.myPosition;
	currentPoint.y += myFont->GetLineHeight();
	CE_Vector2f tempPoint = currentPoint;

	for (int i = 0; i < aTextData.myString.Lenght() + 1; ++i)
	{
		const CE_GlyphData& glyph = myFont->GetGlyphData(aTextData.myString[i]);
		quadData.mySize.x = (float)glyph.mySize.x;
		quadData.mySize.y = (float)glyph.mySize.y;
		quadData.myTexture = glyph.myTexture;

		tempPoint.y -= glyph.myBoundingBox.w;

		quadData.myPosition = tempPoint;
		quadData.myColor = { 0.f, 0.f, 0.f, 1.f };
		RenderQuad(quadData);

		tempPoint.y += 1.f;
		tempPoint.x -= 1.f;
		quadData.myPosition = tempPoint;
		//quadData.myColor = { 1.f, 1.f, 1.f, 1.f };
		quadData.myColor = aTextData.myColor;
		RenderQuad(quadData);

		currentPoint.x += glyph.myAdvance;
		tempPoint = currentPoint;
	}
}

void CE_Renderer::RenderQuad(const CE_2DData& someQuadData)
{
	CE_Vector2f topLeft;
	topLeft.x = someQuadData.myPosition.x;
	topLeft.y = -someQuadData.myPosition.y - someQuadData.mySize.y;
	CE_Vector2f bottomRight = topLeft;
	bottomRight.x += someQuadData.mySize.x;
	bottomRight.y += someQuadData.mySize.y;

	CE_Pos_UV_Vert vertices[4];
	vertices[0].myPosition = CE_Vector4f(topLeft.x, topLeft.y, 0.0f, 1.f); //topleft
	vertices[0].myTexCoord = CE_Vector2f(0.f, 1.f);

	vertices[1].myPosition = CE_Vector4f(bottomRight.x, topLeft.y, 0.0f, 1.f); //topright
	vertices[1].myTexCoord = CE_Vector2f(1.f, 1.f);

	vertices[2].myPosition = CE_Vector4f(topLeft.x, bottomRight.y, 0.0f, 1.f); //bottomleft
	vertices[2].myTexCoord = CE_Vector2f(0.f, 0.f);

	vertices[3].myPosition = CE_Vector4f(bottomRight.x, bottomRight.y, 0.0f, 1.f); //bottomright
	vertices[3].myTexCoord = CE_Vector2f(1.f, 0.f);
	myQuad->UpdateVertexBuffer(vertices, 4, sizeof(CE_Pos_UV_Vert));


	CE_SetResetBlend blend(ALPHA_BLEND);
	CE_SetResetDepth depth(NO_READ_NO_WRITE);

	if (someQuadData.myType == CE_2DData::TEXT)
		myTextShader->Activate();
	else if (someQuadData.myType == CE_2DData::SPRITE)
		mySpriteShader->Activate();

	CE_SpriteShaderData spriteData;
	spriteData.myColor = someQuadData.myColor;
	myQuad->SetObjectData(&spriteData, sizeof(spriteData));

	ID3D11DeviceContext* context = myGPUContext.GetContext();
	ID3D11ShaderResourceView* resource = myEmptyTexture->GetShaderView();

	if (const CE_Texture* texture = someQuadData.myTexture)
		resource = texture->GetShaderView();

	context->PSSetShaderResources(0, 1, &resource);
	myQuad->Render();
}

void CE_Renderer::Render2DLine(const CE_2DData& aLineData)
{
	CE_SetResetBlend blend(ALPHA_BLEND);
	CE_SetResetDepth depth(NO_READ_NO_WRITE);

	myLine2DShader->Activate();

	CE_Line line;
	line.myStart.x = aLineData.myPosition.x;
	line.myStart.y = aLineData.myPosition.y;

	line.myEnd.x = aLineData.mySize.x;
	line.myEnd.y = aLineData.mySize.y;

	line.myStartColor = aLineData.myColor;
	line.myEndColor = aLineData.myColor;

	myLineObject->SetLine(line, myGPUContext);
	myLineObject->Render(myGPUContext);
};