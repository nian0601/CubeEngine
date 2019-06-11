#include "stdafx.h"
#include "CE_Renderer.h"
#include "CE_Text.h"
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

CE_Renderer::CE_Renderer(CE_GPUContext& anGPUContext)
	: myGPUContext(anGPUContext)
{
	CE_ShaderManager* shaderManager = CE_ShaderManager::GetInstance();
	CE_GenericShader* textVX = shaderManager->GetShader("Text.vx");
	CE_GenericShader* textPX = shaderManager->GetShader("Text.px");
	myTextShader = new CE_ShaderPair(textVX, textPX);

	myText = new CE_Text(myGPUContext);
	myText->Init();

	myLineObject = new CE_LineRenderObject();

	myCubeModel = new CE_RenderObject();
	myCubeModel->InitCube();

	mySphereModel = new CE_RenderObject();
	mySphereModel->InitSphere();

	mySprite = new CE_RenderObject();
	mySprite->InitSprite();
	mySprite->CreateObjectData(1, sizeof(CE_SpriteShaderData));
	
	myViewProjectionConstantBuffer = new CE_ConstantBuffer();
	myViewProjectionConstantBuffer->Init(0, sizeof(CE_ViewProjectionData));

	myOrthagonalConstantBuffer = new CE_ConstantBuffer();
	myOrthagonalConstantBuffer->Init(0, sizeof(CE_ProjectionData));

	myModelObjectDataConstantBuffer = new CE_ConstantBuffer();
	myModelObjectDataConstantBuffer->Init(1, sizeof(CE_ModelShaderData));


	CE_GenericShader* cubeVX = shaderManager->GetShader("Cube.vx");
	CE_GenericShader* cubePX = shaderManager->GetShader("Cube.px");
	myCubeShader = new CE_ShaderPair(cubeVX, cubePX);

	CE_GenericShader* lineVX = shaderManager->GetShader("Line.vx");
	CE_GenericShader* linePX = shaderManager->GetShader("Line.px");
	myLineShader = new CE_ShaderPair(lineVX, linePX);

	CE_GenericShader* line2DVX = shaderManager->GetShader("Line2D.vx");
	myLine2DShader = new CE_ShaderPair(line2DVX, linePX);

	CE_GenericShader* spriteVX = shaderManager->GetShader("Sprite.vx");
	CE_GenericShader* spritePX = shaderManager->GetShader("Sprite.px");
	mySpriteShader = new CE_ShaderPair(spriteVX, spritePX);

	myEmptyTexture = CE_TextureManager::GetInstance()->GetEmptyTexture();
}

CE_Renderer::~CE_Renderer()
{
	CE_SAFE_DELETE(myModelObjectDataConstantBuffer);
	CE_SAFE_DELETE(myOrthagonalConstantBuffer);
	CE_SAFE_DELETE(myViewProjectionConstantBuffer);

	CE_SAFE_DELETE(mySprite);
	CE_SAFE_DELETE(mySphereModel);

	CE_SAFE_DELETE(myCubeModel);

	CE_SAFE_DELETE(myLineObject);

	CE_SAFE_DELETE(myText);

	CE_SAFE_DELETE(myTextShader);
	CE_SAFE_DELETE(myLineShader);
	CE_SAFE_DELETE(myLine2DShader);
	CE_SAFE_DELETE(mySpriteShader);

	CE_SAFE_DELETE(myCubeShader);
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
	for (const CE_2DData& data : aRendererProxy.Get2DData())
	{
		if (data.myType == CE_2DData::SPRITE)
			RenderSprite(data);
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
	if (aTextData.myString.Empty())
		return;

	CE_SetResetBlend blend(ALPHA_BLEND);
	CE_SetResetDepth depth(NO_READ_NO_WRITE);

	CE_ShaderPair* shader = myTextShader;
	CE_ASSERT(shader != nullptr, "We dont have a textshader????");

	myOrthagonalConstantBuffer->SendToGPU();
	shader->Activate();

	myText->SetPosition(aTextData.myPosition);
	myText->SetText(aTextData.myString);
	myText->SetColor(aTextData.myColor);
	myText->Render();
}

void CE_Renderer::RenderSprite(const CE_2DData& aSpriteData)
{
	CE_Vector2f topLeft;
	topLeft.x = aSpriteData.myPosition.x - mySceeenSize.x;
	topLeft.y = -aSpriteData.myPosition.y + mySceeenSize.y;
	topLeft.y -= aSpriteData.mySize.y;
	CE_Vector2f bottomRight = topLeft + aSpriteData.mySize;
	topLeft /= mySceeenSize;
	bottomRight /= mySceeenSize;

	CE_Pos_UV_Vert vertices[4];
	vertices[0].myPosition = CE_Vector4f(topLeft.x, topLeft.y, 0.0f, 1.f); //topleft
	vertices[0].myTexCoord = CE_Vector2f(0.f, 1.f);

	vertices[1].myPosition = CE_Vector4f(bottomRight.x, topLeft.y, 0.0f, 1.f); //topright
	vertices[1].myTexCoord = CE_Vector2f(1.f, 1.f);

	vertices[2].myPosition = CE_Vector4f(topLeft.x, bottomRight.y, 0.0f, 1.f); //bottomleft
	vertices[2].myTexCoord = CE_Vector2f(0.f, 0.f);

	vertices[3].myPosition = CE_Vector4f(bottomRight.x, bottomRight.y, 0.0f, 1.f); //bottomright
	vertices[3].myTexCoord = CE_Vector2f(1.f, 0.f);
	mySprite->UpdateVertexBuffer(vertices, 4, sizeof(CE_Pos_UV_Vert));


	CE_SetResetBlend blend(ALPHA_BLEND);
	CE_SetResetDepth depth(NO_READ_NO_WRITE);

	myOrthagonalConstantBuffer->SendToGPU();
	mySpriteShader->Activate();

	CE_SpriteShaderData spriteData;

	spriteData.myColor = aSpriteData.myColor;
	//spriteData.myPosition = aSpriteData.myPosition;
	//spriteData.mySize.x = aSpriteData.mySizeAndHotspot.x * 0.5f;
	//spriteData.mySize.y = aSpriteData.mySizeAndHotspot.y * 0.5f;
	//spriteData.myHotspot.x = aSpriteData.mySizeAndHotspot.z * 2.f;
	//spriteData.myHotspot.y = aSpriteData.mySizeAndHotspot.w * 2.f;

	mySprite->SetObjectData(&spriteData, sizeof(spriteData));

	ID3D11DeviceContext* context = myGPUContext.GetContext();
	ID3D11ShaderResourceView* resource = myEmptyTexture->GetShaderView();

	if (const CE_Texture* texture = aSpriteData.myTexture)
		resource = texture->GetShaderView();

	context->PSSetShaderResources(0, 1, &resource);
	mySprite->Render();
}

void CE_Renderer::Render2DLine(const CE_2DData& aLineData)
{
	CE_SetResetBlend blend(ALPHA_BLEND);
	CE_SetResetDepth depth(NO_READ_NO_WRITE);

	myOrthagonalConstantBuffer->SendToGPU();
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
}
