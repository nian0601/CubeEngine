#include "stdafx.h"
#include "CE_Renderer.h"
#include "CE_CubeShader.h"
#include "CE_Model.h"
#include "CE_SpriteShader.h"
#include "CE_Sprite.h"
#include "CE_TextShader.h"
#include "CE_Text.h"
#include "CE_DirextXFactory.h"


CE_Renderer::CE_Renderer(CE_GPUContext& anGPUContext)
	: myGPUContext(anGPUContext)
{
	myCubeShader = new CE_CubeShader();
	myCubeShader->Init(L"Data/Shaders/Cube.ce_shader", myGPUContext);

	myCubeModel = new CE_Model();
	myCubeModel->InitCube(myGPUContext);

	mySpriteShader = new CE_SpriteShader();
	mySpriteShader->Init(L"Data/Shaders/Sprite.ce_shader", myGPUContext);

	mySprite = new CE_Sprite();
	mySprite->Init(myGPUContext);

	myTextShader = new CE_TextShader();
	myTextShader->Init(L"Data/Shaders/Text.ce_shader", myGPUContext);

	myText = new CE_Text();
	myText->Init(myGPUContext);


	//myMSDFTextShader = new CE_TextShader();
	//myMSDFTextShader->Init(L"Data/Shaders/MSDFText.ce_shader", myGPUContext);
	//
	//myMSDFText = new CE_Text();
	//myMSDFText->InitMSDF(myGPUContext);
}


CE_Renderer::~CE_Renderer()
{
	CE_SAFE_DELETE(myMSDFText);
	CE_SAFE_DELETE(myMSDFTextShader);

	CE_SAFE_DELETE(myText);
	CE_SAFE_DELETE(myTextShader);

	CE_SAFE_DELETE(mySprite);
	CE_SAFE_DELETE(mySpriteShader);

	CE_SAFE_DELETE(myCubeModel);
	CE_SAFE_DELETE(myCubeShader);
}

void CE_Renderer::Render(CE_Camera& aCamera)
{
	CE_DirextXFactory* factory = CE_DirextXFactory::GetInstance();
	factory->SetRasterizerState(CULL_BACK);
	factory->SetDepthStencilState(ENABLED);
	factory->SetBlendState(NO_BLEND);

	myCubeShader->SetGlobalGPUData(myGPUContext, aCamera);

	for (const CubeData& data : myCubeData)
	{
		myCubeModel->SetOrientation(data.myOrientation);
		myCubeModel->SetColor(data.myColor);
		myCubeModel->SetScale(data.myScale);

		myCubeModel->Render(myGPUContext);
	}

	myCubeData.RemoveAll();

	factory->SetBlendState(ALPHA_BLEND);
	mySpriteShader->SetGlobalGPUData(myGPUContext, aCamera);
	for (const SpriteData& data : mySpriteData)
	{
		mySprite->SetPosition(data.myPosition);
		mySprite->SetSize(data.mySize);
		mySprite->SetColor(data.myColor);
		mySprite->SetHotspot(data.myHotspot);
		mySprite->Render(myGPUContext);
	}
	mySpriteData.RemoveAll();

	

	if (myMSDFTextShader != nullptr)
	{
		myMSDFTextShader->SetGlobalGPUData(myGPUContext, aCamera);
		myMSDFText->Render(myGPUContext);
	}
	else
	{
		myTextShader->SetGlobalGPUData(myGPUContext, aCamera);
		myText->Render(myGPUContext);
	}
}

void CE_Renderer::AddCubeData(const CE_Matrix44f& anOrientation, const CE_Vector3f& aScale, const CE_Vector4f& aColor)
{
	CubeData& data = myCubeData.Add();
	data.myOrientation = anOrientation;
	data.myScale = aScale;
	data.myColor = aColor;
}

void CE_Renderer::AddSpriteData(const CE_Vector2f& aPosition, const CE_Vector2f& aSize, const CE_Vector4f& aColor, const CE_Vector2f& aHotspot)
{
	SpriteData& data = mySpriteData.Add();
	data.myPosition = aPosition;
	data.mySize = aSize;
	data.myColor = aColor;
	data.myHotspot = aHotspot;
}
