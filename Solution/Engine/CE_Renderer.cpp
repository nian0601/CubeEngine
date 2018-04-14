#include "stdafx.h"
#include "CE_Renderer.h"
#include "CE_Text.h"
#include "CE_DirextXFactory.h"
#include "CE_RendererProxy.h"
#include "CE_LineRenderObject.h"
#include "CE_Shader.h"
#include "CE_Camera.h"
#include "CE_RenderObject.h"
#include "CE_ShaderStructs.h"
#include "CE_ConstantBuffer.h"

CE_Renderer::CE_Renderer(CE_GPUContext& anGPUContext)
	: myGPUContext(anGPUContext)
{
	myModelShader = new CE_Shader(L"Data/Shaders/Cube.ce_shader", myGPUContext);
	mySpriteShader = new CE_Shader(L"Data/Shaders/Sprite.ce_shader", myGPUContext);
	myLineShader = new CE_Shader(L"Data/Shaders/Line.ce_shader", myGPUContext);
	myTextShader = new CE_Shader(L"Data/Shaders/Text.ce_shader", myGPUContext);
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
	CE_SAFE_DELETE(myModelShader);
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
	RenderSprites(aRendererProxy);
	RenderTexts(aRendererProxy);
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
	myModelShader->Activate();

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

void CE_Renderer::RenderSprites(const CE_RendererProxy& aRendererProxy)
{
	CE_SetResetBlend blend(ALPHA_BLEND);

	myOrthagonalConstantBuffer->SendToGPU();
	mySpriteShader->Activate();

	for (const CE_SpriteData& data : aRendererProxy.GetSpriteData())
	{
		CE_SpriteShaderData* spriteData = mySprite->GetObjectData<CE_SpriteShaderData>();

		spriteData->myColor = data.myColor;
		spriteData->mySize = data.mySize * 0.5f;
		spriteData->myPosition = data.myPosition;
		spriteData->myHotspot = data.myHotspot * 2.f;

		mySprite->Render();
	}
}

void CE_Renderer::RenderTexts(const CE_RendererProxy& aRendererProxy)
{
	CE_SetResetBlend blend(ALPHA_BLEND);

	CE_Shader* shader = myMSDFTextShader;
	if (shader == nullptr)
		shader = myTextShader;

	CE_ASSERT(shader != nullptr, "We dont have a textshader????");

	myOrthagonalConstantBuffer->SendToGPU();
	shader->Activate();

	for (const CE_TextData& data : aRendererProxy.GetTextData())
	{
		if(data.myString.Empty())
			continue;

		if (myMSDFTextShader != nullptr)
		{
			myMSDFText->SetText(data.myString);
			myMSDFText->SetPosition(data.myPosition);
			myMSDFText->Render();
		}
		else
		{
			myText->SetText(data.myString);
			myText->SetPosition(data.myPosition);
			myText->Render();
		}
	}
}
