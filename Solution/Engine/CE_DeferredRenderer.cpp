#include "stdafx.h"

#include "CE_DeferredRenderer.h"
#include "CE_Texture.h"
#include <d3d11.h>
#include "CE_GPUContext.h"
#include "CE_GBuffer.h"
#include "CE_DirextXFactory.h"
#include "CE_RenderObject.h"
#include "CE_ShaderStructs.h"
#include "CE_Camera.h"
#include "CE_RendererProxy.h"
#include "CE_Renderer.h"
#include "CE_ConstantBuffer.h"
#include "CE_ShaderPair.h"
#include "CE_ShaderManager.h"

CE_DeferredRenderer::CE_DeferredRenderer(CE_GPUContext& aGPUContext, CE_Texture& aBackBuffer, const CE_Vector2i& aWindowSize)
	: myGPUContext(aGPUContext)
	, myBackbuffer(aBackBuffer)
{
	myGBuffer = new CE_GBuffer(aGPUContext, aWindowSize);

	myDefferedConstantBuffer = new CE_ConstantBuffer(myGPUContext);
	myDefferedConstantBuffer->Init(sizeof(CE_GlobalPBLData), 0);

	myCubeMap = new CE_Texture();
	myCubeMap->LoadDDS("Data/Textures/church_cubemap.dds", myGPUContext);

	mySSAORandomTexture = new CE_Texture();
	mySSAORandomTexture->LoadDDS("Data/Textures/ssao_random_texture.dds", myGPUContext);

	myQuad = new CE_RenderObject();
	myQuad->InitFullscreenQuad(myGPUContext);

	myPointLightModel = new CE_RenderObject();
	myPointLightModel->InitLightSphere(myGPUContext);
	myPointLightModel->CreateObjectData(sizeof(CE_PointLightShaderData), 1);


	CE_ShaderManager* shaderManager = CE_ShaderManager::GetInstance();
	CE_GenericShader* fullscreenVX = shaderManager->GetShader("FullscreenQuad.vx");
	CE_GenericShader* fullscreenPX = shaderManager->GetShader("FullscreenQuad.px");
	myFullscreenShader = new CE_ShaderPair(fullscreenVX, fullscreenPX);

	CE_GenericShader* pointlightVX = shaderManager->GetShader("Pointlight.vx");
	CE_GenericShader* pointlightPX = shaderManager->GetShader("Pointlight.px");
	myPointLightShader = new CE_ShaderPair(pointlightVX, pointlightPX);
}


CE_DeferredRenderer::~CE_DeferredRenderer()
{
	CE_SAFE_DELETE(myPointLightModel);
	CE_SAFE_DELETE(myQuad);

	CE_SAFE_DELETE(mySSAORandomTexture);
	CE_SAFE_DELETE(myCubeMap);

	CE_SAFE_DELETE(myDefferedConstantBuffer);

	CE_SAFE_DELETE(myGBuffer);

	CE_SAFE_DELETE(myPointLightShader);
	CE_SAFE_DELETE(myFullscreenShader);
}

void CE_DeferredRenderer::ClearGBuffer(const CE_Vector3f& aClearColor)
{
	myGBuffer->Clear(myGPUContext, aClearColor);
}

void CE_DeferredRenderer::UpdateConstantBuffers(const CE_Camera& aCamera)
{
	CE_GlobalPBLData shaderData;
	shaderData.myView = aCamera.GetView();
	shaderData.myProjection = aCamera.GetProjection();
	shaderData.myInvertedProjection = aCamera.GetInvertedProjection();
	shaderData.myNotInvertedView = aCamera.GetNotInvertedView();
	shaderData.myCameraPosition = aCamera.GetNotInvertedView().GetPos();

	CE_Vector2f screenSize;
	screenSize.x = static_cast<float>(myGBuffer->myScreenSize.x);
	screenSize.y = static_cast<float>(myGBuffer->myScreenSize.y);
	shaderData.myScreenSize = screenSize;
	myDefferedConstantBuffer->Update(&shaderData, sizeof(shaderData));
}

void CE_DeferredRenderer::Render(CE_Renderer& aRenderer, const CE_RendererProxy& aRendererProxy)
{
	BeginGBuffer();
	aRenderer.Render3D(aRendererProxy);
	EndGBuffer();
	
	myDefferedConstantBuffer->SendToGPU();

	RenderToScreen();
	RenderPointLights(aRendererProxy);
}

void CE_DeferredRenderer::RenderPointLights(const CE_RendererProxy& aRendererProxy)
{
	CE_SetResetRasterizer raster(NO_CULLING);
	CE_SetResetDepth depth(READ_NO_WRITE);
	CE_SetResetBlend blend(LIGHT_BLEND);

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

void CE_DeferredRenderer::RenderToScreen()
{
	CE_SetResetDepth depth(NO_READ_NO_WRITE);
	CE_SetResetBlend blend(NO_BLEND);
	CE_SetResetRasterizer rasterizer(CULL_BACK);

	myFullscreenShader->Activate();
	myGBuffer->SendToGPU(myGPUContext);

	ID3D11DeviceContext* context = myGPUContext.GetContext();
	ID3D11ShaderResourceView* resources[2];
	resources[0] = mySSAORandomTexture->GetShaderView();
	resources[1] = myCubeMap->GetShaderView();
	context->PSSetShaderResources(3, 2, resources);
	myQuad->Render();
}

void CE_DeferredRenderer::BeginGBuffer()
{
	ID3D11RenderTargetView* targets[3];
	targets[0] = myGBuffer->myTextures[CE_GBuffer::ALBEDO_METALNESS]->GetRenderTarget();
	targets[1] = myGBuffer->myTextures[CE_GBuffer::NORMAL_ROUGNESS]->GetRenderTarget();
	targets[2] = myGBuffer->myTextures[CE_GBuffer::DEPTH]->GetRenderTarget();

	ID3D11DepthStencilView* stencil = myBackbuffer.GetDepthStencil();

	ID3D11DeviceContext* context = myGPUContext.GetContext();
	context->OMSetRenderTargets(3, targets, stencil);
}

void CE_DeferredRenderer::EndGBuffer()
{
	ID3D11DeviceContext* context = myGPUContext.GetContext();

	ID3D11RenderTargetView* target = myBackbuffer.GetRenderTarget();
	ID3D11DepthStencilView* stencil = myBackbuffer.GetDepthStencil();
	context->OMSetRenderTargets(1, &target, stencil);
}