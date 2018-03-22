#include "stdafx.h"
#include "CE_Renderer.h"
#include "CE_Model.h"
#include "CE_SpriteShader.h"
#include "CE_Sprite.h"
#include "CE_Text.h"
#include "CE_DirextXFactory.h"
#include "CE_RendererProxy.h"
#include "CE_LineRenderObject.h"
#include "CE_Shader.h"
#include "CE_Camera.h"

struct CE_TextShaderData
{
	CE_Matrix44f myProjection;
};

struct CE_CubeShaderData
{
	CE_Matrix44f myView;
	CE_Matrix44f myProjection;
};

CE_Renderer::CE_Renderer(CE_GPUContext& anGPUContext)
	: myGPUContext(anGPUContext)
{
	

	myCubeModel = new CE_Model();
	myCubeModel->InitCube(myGPUContext);

	mySprite = new CE_Sprite();
	mySprite->Init(myGPUContext);

	myText = new CE_Text(myGPUContext);
	myText->Init();

	myLineObject = new CE_LineRenderObject();

	CE_ShaderParameters cubeParams;
	cubeParams.myFilePath = L"Data/Shaders/Cube.ce_shader";
	cubeParams.myInputElements.Add(CE_ShaderParameters::POSITION);
	cubeParams.myInputElements.Add(CE_ShaderParameters::NORMAL);
	cubeParams.myInputElements.Add(CE_ShaderParameters::COLOR);
	myCubeShader = new CE_Shader(cubeParams, myGPUContext);
	myCubeShader->InitGlobalData<CE_CubeShaderData>();

	mySpriteShader = new CE_SpriteShader();
	mySpriteShader->Init(L"Data/Shaders/Sprite.ce_shader", myGPUContext);

	CE_ShaderParameters textParams;
	textParams.myFilePath = L"Data/Shaders/Text.ce_shader";
	textParams.myInputElements.Add(CE_ShaderParameters::POSITION);
	textParams.myInputElements.Add(CE_ShaderParameters::UV);
	myTextShader = new CE_Shader(textParams, myGPUContext);
	myTextShader->InitGlobalData<CE_TextShaderData>();

	//myMSDFTextShader = new CE_TextShader();
	//myMSDFTextShader->Init(L"Data/Shaders/MSDFText.ce_shader", myGPUContext);
	//
	//myMSDFText = new CE_Text();
	//myMSDFText->InitMSDF(myGPUContext);

	//myLineShader = new CE_CubeShader();
	//myLineShader->Init(L"Data/Shaders/Line.ce_shader", myGPUContext, true);

	CE_ShaderParameters lineParams;
	lineParams.myFilePath = L"Data/Shaders/Line.ce_shader";
	lineParams.myInputElements.Add(CE_ShaderParameters::POSITION);
	lineParams.myInputElements.Add(CE_ShaderParameters::COLOR);
	myLineShader = new CE_Shader(lineParams, myGPUContext);
	myLineShader->InitGlobalData<CE_CubeShaderData>();
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

	CE_CubeShaderData* shaderData = myLineShader->GetGlobalData<CE_CubeShaderData>();
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

	CE_CubeShaderData* shaderData = myCubeShader->GetGlobalData<CE_CubeShaderData>();
	shaderData->myView = aCamera.GetView();
	shaderData->myProjection = aCamera.GetProjection();

	myCubeShader->Activate();

	for (const CE_CubeData& data : aRendererProxy.GetCubeData())
	{
		myCubeModel->SetOrientation(data.myOrientation);
		myCubeModel->SetColor(data.myColor);
		myCubeModel->SetScale(data.myScale);

		myCubeModel->Render(myGPUContext);
	}
}

void CE_Renderer::RenderSprites(const CE_Matrix44f& aOrthagonalMatrix, const CE_RendererProxy& aRendererProxy)
{
	CE_SetResetBlend blend(ALPHA_BLEND);

	mySpriteShader->SetGlobalGPUData(myGPUContext, aOrthagonalMatrix);
	for (const CE_SpriteData& data : aRendererProxy.GetSpriteData())
	{
		mySprite->SetPosition(data.myPosition);
		mySprite->SetSize(data.mySize);
		mySprite->SetColor(data.myColor);
		mySprite->SetHotspot(data.myHotspot);
		mySprite->Render(myGPUContext);
	}
}

void CE_Renderer::RenderTexts(const CE_Matrix44f& aOrthagonalMatrix, const CE_RendererProxy& aRendererProxy)
{
	CE_SetResetBlend blend(ALPHA_BLEND);
	CE_SetResetSampler sampler(LINEAR_SAMPLING);

	CE_Shader* shader = myMSDFTextShader;
	if (shader == nullptr)
		shader = myTextShader;

	CE_ASSERT(shader != nullptr, "We dont have a textshader????");

	CE_TextShaderData* shaderData = shader->GetGlobalData<CE_TextShaderData>();
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
