#include "stdafx.h"

#include "CE_DeferredRenderer.h"
#include "CE_Texture.h"
#include <d3d11.h>
#include "CE_GPUContext.h"

CE_DeferredRenderer::GBuffer::GBuffer(CE_GPUContext& aGPUContext, const CE_Vector2i& aWindowSize)
{
	myTextures[ALBEDO_METALNESS] = new CE_Texture();
	myTextures[ALBEDO_METALNESS]->InitForShader(aWindowSize
		, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R8G8B8A8_UNORM, aGPUContext);

	myTextures[NORMAL_ROUGNESS] = new CE_Texture();
	myTextures[NORMAL_ROUGNESS]->InitForShader(aWindowSize
		, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R8G8B8A8_UNORM, aGPUContext);

	myTextures[DEPTH] = new CE_Texture();
	myTextures[DEPTH]->InitForShader(aWindowSize
		, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R32_FLOAT, aGPUContext);

	myTextures[DEPTH_STENCIL] = new CE_Texture();
	myTextures[DEPTH_STENCIL]->InitForShader(aWindowSize
		, D3D11_BIND_DEPTH_STENCIL
		, DXGI_FORMAT_R32_TYPELESS, aGPUContext);
}

CE_DeferredRenderer::GBuffer::~GBuffer()
{
	for (int i = 0; i < __COUNT; ++i)
		CE_SAFE_DELETE(myTextures[i]);
}

void CE_DeferredRenderer::GBuffer::Clear(CE_GPUContext& aGPUContext, const CE_Vector4f& aClearColor)
{
	float clearColor[4];
	clearColor[0] = aClearColor.x;
	clearColor[1] = aClearColor.y;
	clearColor[2] = aClearColor.z;
	clearColor[3] = aClearColor.w;

	ID3D11DeviceContext* context = aGPUContext.GetContext();
	context ->ClearRenderTargetView(myTextures[ALBEDO_METALNESS]->GetRenderTarget(), clearColor);
	context ->ClearRenderTargetView(myTextures[NORMAL_ROUGNESS]->GetRenderTarget(), clearColor);
	context ->ClearRenderTargetView(myTextures[DEPTH]->GetRenderTarget(), clearColor);

	context->ClearDepthStencilView(myTextures[DEPTH_STENCIL]->GetDepthStencil(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

CE_DeferredRenderer::CE_DeferredRenderer(CE_GPUContext& aGPUContext, const CE_Vector2i& aWindowSize)
	: myGPUContext(aGPUContext)
{
	myGBuffer = new GBuffer(aGPUContext, aWindowSize);
}


CE_DeferredRenderer::~CE_DeferredRenderer()
{
	CE_SAFE_DELETE(myGBuffer);
}

void CE_DeferredRenderer::BeginGBuffer()
{
	myGBuffer->Clear(myGPUContext, { 0.15f, 0.15f, 0.15f, 1.f });

	ID3D11RenderTargetView* targets[3];
	targets[0] = myGBuffer->myTextures[GBuffer::ALBEDO_METALNESS]->GetRenderTarget();
	targets[1] = myGBuffer->myTextures[GBuffer::NORMAL_ROUGNESS]->GetRenderTarget();
	targets[2] = myGBuffer->myTextures[GBuffer::DEPTH]->GetRenderTarget();

	ID3D11DepthStencilView* stencil = myGBuffer->myTextures[GBuffer::DEPTH_STENCIL]->GetDepthStencil();

	ID3D11DeviceContext* context = myGPUContext.GetContext();
	context->OMSetRenderTargets(3, targets, stencil);
}

void CE_DeferredRenderer::EndGBuffer()
{

}

void CE_DeferredRenderer::RenderToScreen()
{

}