#include "stdafx.h"

#include "CE_DeferredRenderer.h"
#include "CE_Texture.h"
#include <d3d11.h>
#include "CE_GPUContext.h"
#include "CE_GBuffer.h"
#include "CE_DirextXFactory.h"
#include "CE_Shader.h"
#include "CE_RenderObject.h"
#include "CE_ShaderStructs.h"
#include "CE_Camera.h"
#include "CE_RendererProxy.h"


CE_DeferredRenderer::CE_DeferredRenderer(CE_GPUContext& aGPUContext, const CE_Vector2i& aWindowSize)
	: myGPUContext(aGPUContext)
{
	myGBuffer = new CE_GBuffer(aGPUContext, aWindowSize);

	CE_ShaderParameters shaderParams;
	shaderParams.myFilePath = L"Data/Shaders/FullscreenQuad.ce_shader";
	shaderParams.myInputElements.Add(CE_ShaderParameters::POSITION);
	shaderParams.myInputElements.Add(CE_ShaderParameters::UV);
	myShader = new CE_Shader(shaderParams, myGPUContext);
	myShader->InitGlobalData<CE_GlobalPBLData>();

	myQuad = new CE_RenderObject();
	myQuad->InitFullscreenQuad(myGPUContext);

	myCubeMap = new CE_Texture();
	myCubeMap->LoadDDS("Data/Textures/church_cubemap.dds", myGPUContext);


	CE_ShaderParameters pointLightParams;
	pointLightParams.myFilePath = L"Data/Shaders/Pointlight.ce_shader";
	pointLightParams.myInputElements.Add(CE_ShaderParameters::POSITION);
	myPointLightShader = new CE_Shader(pointLightParams, myGPUContext);
	myPointLightShader->InitGlobalData<CE_GlobalPBLData>();

	myPointLightModel = new CE_RenderObject();
	myPointLightModel->InitLightSphere(myGPUContext);
	myPointLightModel->CreateObjectData(sizeof(CE_PointLightShaderData), 1);
}


CE_DeferredRenderer::~CE_DeferredRenderer()
{
	CE_SAFE_DELETE(myCubeMap);
	CE_SAFE_DELETE(myQuad);
	CE_SAFE_DELETE(myGBuffer);

	CE_SAFE_DELETE(myPointLightShader);
	CE_SAFE_DELETE(myPointLightModel);
}

void CE_DeferredRenderer::BeginGBuffer(CE_Texture* aBackbuffer)
{
	myGBuffer->Clear(myGPUContext, { 0.f, 0.f, 0.f, 1.f });

	ID3D11RenderTargetView* targets[3];
	targets[0] = myGBuffer->myTextures[CE_GBuffer::ALBEDO_METALNESS]->GetRenderTarget();
	targets[1] = myGBuffer->myTextures[CE_GBuffer::NORMAL_ROUGNESS]->GetRenderTarget();
	targets[2] = myGBuffer->myTextures[CE_GBuffer::DEPTH]->GetRenderTarget();

	ID3D11DepthStencilView* stencil = aBackbuffer->GetDepthStencil();

	ID3D11DeviceContext* context = myGPUContext.GetContext();
	context->OMSetRenderTargets(3, targets, stencil);
}

void CE_DeferredRenderer::EndGBuffer(CE_Texture* aBackbuffer)
{
	ID3D11DeviceContext* context = myGPUContext.GetContext();

	ID3D11RenderTargetView* target = aBackbuffer->GetRenderTarget();
	ID3D11DepthStencilView* stencil = aBackbuffer->GetDepthStencil();
	context->OMSetRenderTargets(1, &target, stencil);
}

void CE_DeferredRenderer::RenderPointLights(const CE_Camera& aCamera, const CE_RendererProxy& aRendererProxy)
{
	CE_SetResetRasterizer raster(NO_CULLING);
	CE_SetResetDepth depth(READ_NO_WRITE);
	CE_SetResetBlend blend(LIGHT_BLEND);

	CE_GlobalPBLData* shaderData = myPointLightShader->GetGlobalData<CE_GlobalPBLData>();
	shaderData->myView = aCamera.GetView();
	shaderData->myProjection = aCamera.GetProjection();
	shaderData->myInvertedProjection = aCamera.GetInvertedProjection();
	shaderData->myNotInvertedView = aCamera.GetNotInvertedView();
	shaderData->myCameraPosition = aCamera.GetNotInvertedView().GetPos();

	myPointLightShader->Activate();
	myGBuffer->SendToGPU(myGPUContext);

	for (const CE_PointLightData& data : aRendererProxy.GetPointLightData())
	{
		CE_PointLightShaderData* modelData = myPointLightModel->GetObjectData<CE_PointLightShaderData>();
		modelData->myWorld = data.myOrientation;
		modelData->myColorAndIntensity = data.myColorAndIntensity;
		modelData->myRadius = CE_Vector4f(data.myRadius);

		myPointLightModel->Render();
	}
}

void CE_DeferredRenderer::RenderToScreen(CE_Camera* aCamera)
{
	CE_SetResetDepth depth(NO_READ_NO_WRITE);
	CE_SetResetBlend blend(NO_BLEND);
	CE_SetResetRasterizer rasterizer(CULL_BACK);

	CE_GlobalPBLData* pblData = myShader->GetGlobalData<CE_GlobalPBLData>();
	pblData->myInvertedProjection = aCamera->GetInvertedProjection();
	pblData->myNotInvertedView = aCamera->GetNotInvertedView();
	pblData->myCameraPosition = aCamera->GetNotInvertedView().GetPos();

	myShader->Activate();
	myGBuffer->SendToGPU(myGPUContext);

	ID3D11DeviceContext* context = myGPUContext.GetContext();
	ID3D11ShaderResourceView* resource = myCubeMap->GetShaderView();
	context->PSSetShaderResources(3, 1, &resource);
	myQuad->Render();
}