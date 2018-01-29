#include "stdafx.h"
#include "CE_Renderer.h"
#include "CE_CubeShader.h"
#include "CE_Model.h"
#include "CE_SpriteShader.h"
#include "CE_Sprite.h"
#include "CE_TextShader.h"
#include "CE_Text.h"
#include "CE_DirextXFactory.h"
#include "CE_RendererProxy.h"
#include "CE_LineRenderObject.h"


CE_Renderer::CE_Renderer(CE_GPUContext& anGPUContext)
	: myGPUContext(anGPUContext)
{
	myCubeShader = new CE_CubeShader();
	myCubeShader->Init(L"Data/Shaders/Cube.ce_shader", myGPUContext, false);

	myCubeModel = new CE_Model();
	myCubeModel->InitCube(myGPUContext);

	mySpriteShader = new CE_SpriteShader();
	mySpriteShader->Init(L"Data/Shaders/Sprite.ce_shader", myGPUContext);

	mySprite = new CE_Sprite();
	mySprite->Init(myGPUContext);

	myTextShader = new CE_TextShader();
	myTextShader->Init(L"Data/Shaders/Text.ce_shader", myGPUContext);

	myText = new CE_Text(myGPUContext);
	myText->Init();

	//myMSDFTextShader = new CE_TextShader();
	//myMSDFTextShader->Init(L"Data/Shaders/MSDFText.ce_shader", myGPUContext);
	//
	//myMSDFText = new CE_Text();
	//myMSDFText->InitMSDF(myGPUContext);


	myLineShader = new CE_CubeShader();
	myLineShader->Init(L"Data/Shaders/Line.ce_shader", myGPUContext, true);

	myLineObject = new CE_LineRenderObject();
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

	CE_DirextXFactory* factory = CE_DirextXFactory::GetInstance();
	factory->SetBlendState(ALPHA_BLEND);
	factory->SetDepthStencilState(ENABLED);

	myLineShader->SetGlobalGPUData(myGPUContext, aCamera);
	
	myLineObject->SetLines(someLines, myGPUContext);
	myLineObject->Render(myGPUContext);
}

void CE_Renderer::RenderCubes(CE_Camera& aCamera, const CE_RendererProxy& aRendererProxy)
{
	CE_DirextXFactory* factory = CE_DirextXFactory::GetInstance();
	factory->SetRasterizerState(CULL_BACK);
	factory->SetDepthStencilState(ENABLED);
	factory->SetBlendState(NO_BLEND);

	myCubeShader->SetGlobalGPUData(myGPUContext, aCamera);

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
	CE_DirextXFactory* factory = CE_DirextXFactory::GetInstance();
	factory->SetBlendState(ALPHA_BLEND);

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
	CE_DirextXFactory* factory = CE_DirextXFactory::GetInstance();
	factory->SetBlendState(ALPHA_BLEND);

	if (myMSDFTextShader != nullptr)
		myMSDFTextShader->SetGlobalGPUData(myGPUContext, aOrthagonalMatrix);
	else
		myTextShader->SetGlobalGPUData(myGPUContext, aOrthagonalMatrix);


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
