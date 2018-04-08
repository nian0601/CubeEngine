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

CE_Renderer::CE_Renderer(CE_GPUContext& anGPUContext)
	: myGPUContext(anGPUContext)
{
	myText = new CE_Text(myGPUContext);
	myText->Init();

	myLineObject = new CE_LineRenderObject();

	{
		CE_ShaderParameters cubeParams;
		cubeParams.myFilePath = L"Data/Shaders/Cube.ce_shader";
		cubeParams.myInputElements.Add(CE_ShaderParameters::POSITION);
		cubeParams.myInputElements.Add(CE_ShaderParameters::NORMAL);
		cubeParams.myInputElements.Add(CE_ShaderParameters::COLOR);
		myCubeShader = new CE_Shader(cubeParams, myGPUContext);
		myCubeShader->InitGlobalData<CE_ViewProjectionData>();
	}

	{
		CE_ShaderParameters spriteParams;
		spriteParams.myFilePath = L"Data/Shaders/Sprite.ce_shader";
		spriteParams.myInputElements.Add(CE_ShaderParameters::POSITION);
		mySpriteShader = new CE_Shader(spriteParams, myGPUContext);
		mySpriteShader->InitGlobalData<CE_ProjectionData>();
	}


	{
		CE_ShaderParameters textParams;
		textParams.myFilePath = L"Data/Shaders/Text.ce_shader";
		textParams.myInputElements.Add(CE_ShaderParameters::POSITION);
		textParams.myInputElements.Add(CE_ShaderParameters::UV);
		myTextShader = new CE_Shader(textParams, myGPUContext);
		myTextShader->InitGlobalData<CE_ProjectionData>();
		//myMSDFTextShader = new CE_TextShader();
		//myMSDFTextShader->Init(L"Data/Shaders/MSDFText.ce_shader", myGPUContext);
		//
		//myMSDFText = new CE_Text();
		//myMSDFText->InitMSDF(myGPUContext);
	}

	{
		CE_ShaderParameters lineParams;
		lineParams.myFilePath = L"Data/Shaders/Line.ce_shader";
		lineParams.myInputElements.Add(CE_ShaderParameters::POSITION);
		lineParams.myInputElements.Add(CE_ShaderParameters::COLOR);
		myLineShader = new CE_Shader(lineParams, myGPUContext);
		myLineShader->InitGlobalData<CE_ViewProjectionData>();
	}


	myCubeModel = new CE_RenderObject();
	//myCubeModel->InitCube(myGPUContext);
	myCubeModel->InitSphere(myGPUContext);
	myCubeModel->CreateObjectData(sizeof(CE_ModelShaderData), 1);

	mySprite = new CE_RenderObject();
	mySprite->InitSprite(myGPUContext);
	mySprite->CreateObjectData(sizeof(CE_SpriteShaderData), 1);
}


CE_Renderer::~CE_Renderer()
{
	CE_SAFE_DELETE(myLineObject);
	CE_SAFE_DELETE(myLineShader);

	CE_SAFE_DELETE(myMSDFText);
	CE_SAFE_DELETE(myMSDFTextShader);

	CE_SAFE_DELETE(myText);
	CE_SAFE_DELETE(myTextShader);

	CE_SAFE_DELETE(mySprite);
	CE_SAFE_DELETE(mySpriteShader);

	CE_SAFE_DELETE(myCubeModel);
	CE_SAFE_DELETE(myCubeShader);
}

void CE_Renderer::Render3D(CE_Camera& aCamera, const CE_RendererProxy& aRendererProxy)
{
	RenderCubes(aCamera, aRendererProxy);
}

void CE_Renderer::Render2D(const CE_Matrix44f& aOrthagonalMatrix, const CE_RendererProxy& aRendererProxy)
{
	RenderSprites(aOrthagonalMatrix, aRendererProxy);
	RenderTexts(aOrthagonalMatrix, aRendererProxy);
}

void CE_Renderer::RenderLines(CE_Camera& aCamera, const CE_GrowingArray<CE_Line>& someLines)
{
	if (someLines.Size() == 0)
		return;

	CE_SetResetBlend blend(ALPHA_BLEND);
	CE_SetResetDepth depth(ENABLED);

	CE_ViewProjectionData* shaderData = myLineShader->GetGlobalData<CE_ViewProjectionData>();
	shaderData->myView = aCamera.GetView();
	shaderData->myProjection = aCamera.GetProjection();

	myLineShader->Activate();
	
	myLineObject->SetLines(someLines, myGPUContext);
	myLineObject->Render(myGPUContext);
}

void CE_Renderer::RenderCubes(CE_Camera& aCamera, const CE_RendererProxy& aRendererProxy)
{
	CE_SetResetRasterizer raster(CULL_BACK);
	CE_SetResetDepth depth(ENABLED);
	CE_SetResetBlend blend(NO_BLEND);

	CE_ViewProjectionData* shaderData = myCubeShader->GetGlobalData<CE_ViewProjectionData>();
	shaderData->myView = aCamera.GetView();
	shaderData->myProjection = aCamera.GetProjection();

	myCubeShader->Activate();

	for (const CE_CubeData& data : aRendererProxy.GetCubeData())
	{
		CE_CubeData* cubeData = myCubeModel->GetObjectData<CE_CubeData>();
		cubeData->myOrientation = data.myOrientation;
		cubeData->myColorAndMetalness = data.myColorAndMetalness;
		cubeData->myScaleAndRoughness = data.myScaleAndRoughness;

		myCubeModel->Render();
	}
}

void CE_Renderer::RenderSprites(const CE_Matrix44f& aOrthagonalMatrix, const CE_RendererProxy& aRendererProxy)
{
	CE_SetResetBlend blend(ALPHA_BLEND);

	CE_ProjectionData* shaderData = mySpriteShader->GetGlobalData<CE_ProjectionData>();
	shaderData->myProjection = aOrthagonalMatrix;
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

void CE_Renderer::RenderTexts(const CE_Matrix44f& aOrthagonalMatrix, const CE_RendererProxy& aRendererProxy)
{
	CE_SetResetBlend blend(ALPHA_BLEND);

	CE_Shader* shader = myMSDFTextShader;
	if (shader == nullptr)
		shader = myTextShader;

	CE_ASSERT(shader != nullptr, "We dont have a textshader????");

	CE_ProjectionData* shaderData = shader->GetGlobalData<CE_ProjectionData>();
	shaderData->myProjection = aOrthagonalMatrix;
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
