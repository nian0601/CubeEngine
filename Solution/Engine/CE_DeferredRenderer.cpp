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


CE_DeferredRenderer::CE_DeferredRenderer(CE_GPUContext& aGPUContext, const CE_Vector2i& aWindowSize)
	: myGPUContext(aGPUContext)
{
	myGBuffer = new CE_GBuffer(aGPUContext, aWindowSize);

	CE_ShaderParameters shaderParams;
	shaderParams.myFilePath = L"Data/Shaders/FullscreenQuad.ce_shader";
	shaderParams.myInputElements.Add(CE_ShaderParameters::POSITION);
	shaderParams.myInputElements.Add(CE_ShaderParameters::UV);
	myShader = new CE_Shader(shaderParams, myGPUContext);

	myQuad = new CE_RenderObject();
	myQuad->InitFullscreenQuad(myGPUContext);
	myQuad->CreateObjectData(sizeof(CE_FullscreenQuadShaderData), 0);

	myCubeMap = new CE_Texture();
	myCubeMap->LoadDDS("Data/Textures/church_cubemap.dds", myGPUContext);
}


CE_DeferredRenderer::~CE_DeferredRenderer()
{
	CE_SAFE_DELETE(myCubeMap);
	CE_SAFE_DELETE(myQuad);
	CE_SAFE_DELETE(myGBuffer);
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

void CE_DeferredRenderer::RenderToScreen(CE_Camera* aCamera)
{
	CE_SetResetDepth depth(NO_READ_NO_WRITE);
	CE_SetResetBlend blend(NO_BLEND);
	CE_SetResetRasterizer rasterizer(CULL_BACK);

	CE_FullscreenQuadShaderData* quadData = myQuad->GetObjectData<CE_FullscreenQuadShaderData>();
	quadData->myInvertedProjection = aCamera->GetInvertedProjection();
	quadData->myNotInvertedView = aCamera->GetNotInvertedView();
	quadData->myCameraPosition = aCamera->GetNotInvertedView().GetPos();

	myShader->Activate();
	myGBuffer->SendToGPU(myGPUContext);

	ID3D11DeviceContext* context = myGPUContext.GetContext();
	ID3D11ShaderResourceView* resource = myCubeMap->GetShaderView();
	context->PSSetShaderResources(3, 1, &resource);
	myQuad->Render();
}