#include "stdafx.h"
#include "CE_Renderer.h"
#include "CE_Shader.h"
#include "CE_Model.h"


CE_Renderer::CE_Renderer(CE_GPUContext& anGPUContext)
	: myGPUContext(anGPUContext)
{
	myCubeShader = new CE_Shader();
	myCubeShader->Init(L"Data/Shaders/Cube.ce_shader", myGPUContext);

	myCubeModel = new CE_Model();
	myCubeModel->InitCube(myGPUContext);
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

		myCubeModel->Render(myGPUContext);
	}

	myCubeData.RemoveAll();
}

void CE_Renderer::AddCubeData(const CE_Matrix44f& anOrientation, const CE_Vector4f& aColor)
{
	CubeData& data = myCubeData.Add();
	data.myOrientation = anOrientation;
	data.myColor = aColor;
}
