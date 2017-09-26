#include "stdafx.h"
#include "CE_Renderer.h"
#include "CE_CubeShader.h"
#include "CE_Model.h"
#include "CE_SpriteShader.h"
#include "CE_Sprite.h"


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
}


CE_Renderer::~CE_Renderer()
{
	CE_SAFE_DELETE(myCubeModel);
	CE_SAFE_DELETE(myCubeShader);
}

void CE_Renderer::Render(CE_Camera& aCamera)
{
	myCubeShader->SetGlobalGPUData(myGPUContext, aCamera);

	for (const CubeData& data : myCubeData)
	{
		myCubeModel->SetOrientation(data.myOrientation);
		myCubeModel->SetColor(data.myColor);
		myCubeModel->SetScale(data.myScale);

		myCubeModel->Render(myGPUContext);
	}

	myCubeData.RemoveAll();

	mySpriteShader->SetGlobalGPUData(myGPUContext, aCamera);

	
	mySprite->SetSize({ 0.25f, 0.25f });
	mySprite->SetPosition({ 0.5f, 0.5f });
	mySprite->SetHotspot({ 0.5f, 0.5f });
	mySprite->Render(myGPUContext);

	mySprite->SetSize({ 0.10f, 1.f });
	mySprite->SetPosition({ 0.0f, 0.0f });
	mySprite->SetHotspot({ 0.0f, 0.0f });
	mySprite->Render(myGPUContext);

	mySprite->SetSize({ 0.10f, 1.f });
	mySprite->SetPosition({ 1.0f, 0.0f });
	mySprite->SetHotspot({ 1.0f, 0.0f });
	mySprite->Render(myGPUContext);
}

void CE_Renderer::AddCubeData(const CE_Matrix44f& anOrientation, const CE_Vector3f& aScale, const CE_Vector4f& aColor)
{
	CubeData& data = myCubeData.Add();
	data.myOrientation = anOrientation;
	data.myScale = aScale;
	data.myColor = aColor;
}
