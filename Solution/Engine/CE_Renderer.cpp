#include "stdafx.h"
#include "CE_Renderer.h"
#include "CE_Text.h"
#include "CE_DirextXFactory.h"
#include "CE_RendererProxy.h"
#include "CE_LineRenderObject.h"
#include "CE_Camera.h"
#include "CE_RenderObject.h"
#include "CE_ShaderStructs.h"
#include "CE_ConstantBuffer.h"
#include "CE_ShaderPair.h"
#include "CE_ShaderManager.h"

CE_Renderer::CE_Renderer(CE_GPUContext& anGPUContext, CE_ShaderManager& aShaderManager)
	: myGPUContext(anGPUContext)
{
	myTextShader = new CE_ShaderPair("Data/Shaders/Text.vx", "Data/Shaders/Text.px", myGPUContext);
	//myMSDFTextShader = new CE_Shader(L"Data/Shaders/MSDFText.ce_shader", myGPUContext);

	myText = new CE_Text(myGPUContext);
	myText->Init();

	//myMSDFText = new CE_Text();
	//myMSDFText->InitMSDF(myGPUContext);

	myLineObject = new CE_LineRenderObject();

	myCubeModel = new CE_RenderObject();
	myCubeModel->InitCube(myGPUContext);

	mySphereModel = new CE_RenderObject();
	mySphereModel->InitSphere(myGPUContext);

	mySprite = new CE_RenderObject();
	mySprite->InitSprite(myGPUContext);
	mySprite->CreateObjectData(sizeof(CE_SpriteShaderData), 1);
	
	myViewProjectionConstantBuffer = new CE_ConstantBuffer(myGPUContext);
	myViewProjectionConstantBuffer->Init(sizeof(CE_ViewProjectionData), 0);

	myOrthagonalConstantBuffer = new CE_ConstantBuffer(myGPUContext);
	myOrthagonalConstantBuffer->Init(sizeof(CE_ProjectionData), 0);

	myModelObjectDataConstantBuffer = new CE_ConstantBuffer(myGPUContext);
	myModelObjectDataConstantBuffer->Init(sizeof(CE_ModelShaderData), 1);


	CE_GenericShader* cubeVX = aShaderManager.GetShader("Cube.vx");
	CE_GenericShader* cubePX = aShaderManager.GetShader("Cube.px");
	myCubeShader = new CE_ShaderPair(cubeVX, cubePX);

	CE_GenericShader* lineVX = aShaderManager.GetShader("Line.vx");
	CE_GenericShader* linePX = aShaderManager.GetShader("Line.px");
	myLineShader = new CE_ShaderPair(lineVX, linePX);

	CE_GenericShader* spriteVX = aShaderManager.GetShader("Sprite.vx");
	CE_GenericShader* spritePX = aShaderManager.GetShader("Sprite.px");
	mySpriteShader = new CE_ShaderPair(spriteVX, spritePX);
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

	CE_SAFE_DELETE(myMSDFText);
	CE_SAFE_DELETE(myText);

	CE_SAFE_DELETE(myMSDFTextShader);
	CE_SAFE_DELETE(myTextShader);
	CE_SAFE_DELETE(myLineShader);
	CE_SAFE_DELETE(mySpriteShader);

	CE_SAFE_DELETE(myCubeShader);
}

void CE_Renderer::UpdateConstantBuffers(const CE_Camera& aCamera)
{
	CE_ViewProjectionData viewProjection;
	viewProjection.myView = aCamera.GetView();
	viewProjection.myProjection = aCamera.GetProjection();
	myViewProjectionConstantBuffer->Update(&viewProjection, sizeof(viewProjection));

	CE_ProjectionData orthagonal;
	orthagonal.myProjection = aCamera.GetOrthagonalProjection();
	myOrthagonalConstantBuffer->Update(&orthagonal, sizeof(orthagonal));
}

void CE_Renderer::Render3D(const CE_RendererProxy& aRendererProxy)
{
	RenderModels(aRendererProxy);
}

void CE_Renderer::Render2D(const CE_RendererProxy& aRendererProxy)
{
	for (const CE_2DData& data : aRendererProxy.Get2DData())
	{
		if (data.myType == CE_2DData::SPRITE)
			RenderSprite(data);
		else if (data.myType == CE_2DData::TEXT)
			RenderText(data);
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

		myModelObjectDataConstantBuffer->Update(&modelData, sizeof(modelData));
		myModelObjectDataConstantBuffer->SendToGPU();

		model->Render();
	}
}

void CE_Renderer::RenderText(const CE_2DData& aTextData)
{
	if (aTextData.myString.Empty())
		return;

	CE_SetResetBlend blend(ALPHA_BLEND);
	CE_SetResetDepth depth(NO_READ_NO_WRITE);

	CE_ShaderPair* shader = myMSDFTextShader;
	if (shader == nullptr)
		shader = myTextShader;

	CE_ASSERT(shader != nullptr, "We dont have a textshader????");

	myOrthagonalConstantBuffer->SendToGPU();
	shader->Activate();

	if (myMSDFTextShader != nullptr)
	{
		myMSDFText->SetText(aTextData.myString);
		myMSDFText->SetPosition(aTextData.myPosition);
		myMSDFText->SetColor(aTextData.myColor);
		myMSDFText->Render();
	}
	else
	{
		myText->SetText(aTextData.myString);
		myText->SetPosition(aTextData.myPosition);
		myText->SetColor(aTextData.myColor);
		myText->Render();
	}
}

void CE_Renderer::RenderSprite(const CE_2DData& aSpriteData)
{
	CE_SetResetBlend blend(ALPHA_BLEND);
	CE_SetResetDepth depth(NO_READ_NO_WRITE);

	myOrthagonalConstantBuffer->SendToGPU();
	mySpriteShader->Activate();

	CE_SpriteShaderData* spriteData = mySprite->GetObjectData<CE_SpriteShaderData>();

	spriteData->myColor = aSpriteData.myColor;
	spriteData->myPosition = aSpriteData.myPosition;
	spriteData->mySize.x = aSpriteData.mySizeAndHotspot.x * 0.5f;
	spriteData->mySize.y = aSpriteData.mySizeAndHotspot.y * 0.5f;
	spriteData->myHotspot.x = aSpriteData.mySizeAndHotspot.z * 2.f;
	spriteData->myHotspot.y = aSpriteData.mySizeAndHotspot.w * 2.f;

	mySprite->Render();
}
