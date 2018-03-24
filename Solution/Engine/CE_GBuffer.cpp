#include "stdafx.h"

#include "CE_GPUContext.h"
#include "CE_GBuffer.h"
#include "CE_Texture.h"

#include <d3d11.h>


CE_GBuffer::CE_GBuffer(CE_GPUContext& aGPUContext, const CE_Vector2i& aWindowSize)
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

	//myTextures[DEPTH_STENCIL] = new CE_Texture();
	//myTextures[DEPTH_STENCIL]->InitForShader(aWindowSize
	//	, D3D11_BIND_DEPTH_STENCIL
	//	, DXGI_FORMAT_R32_TYPELESS, aGPUContext);
}

CE_GBuffer::~CE_GBuffer()
{
	for (int i = 0; i < __COUNT; ++i)
		CE_SAFE_DELETE(myTextures[i]);
}

void CE_GBuffer::SendToGPU(CE_GPUContext& aGPUContext)
{
	ID3D11DeviceContext* context = aGPUContext.GetContext();

	ID3D11ShaderResourceView* resources[3];
	resources[0] = myTextures[CE_GBuffer::ALBEDO_METALNESS]->GetShaderView();
	resources[1] = myTextures[CE_GBuffer::NORMAL_ROUGNESS]->GetShaderView();
	resources[2] = myTextures[CE_GBuffer::DEPTH]->GetShaderView();

	context->PSSetShaderResources(0, 3, resources);
}

void CE_GBuffer::Clear(CE_GPUContext& aGPUContext, const CE_Vector4f& aClearColor)
{
	float clearColor[4];
	clearColor[0] = aClearColor.x;
	clearColor[1] = aClearColor.y;
	clearColor[2] = aClearColor.z;
	clearColor[3] = aClearColor.w;

	ID3D11DeviceContext* context = aGPUContext.GetContext();
	context->ClearRenderTargetView(myTextures[ALBEDO_METALNESS]->GetRenderTarget(), clearColor);
	context->ClearRenderTargetView(myTextures[NORMAL_ROUGNESS]->GetRenderTarget(), clearColor);
	context->ClearRenderTargetView(myTextures[DEPTH]->GetRenderTarget(), clearColor);

	//context->ClearDepthStencilView(myTextures[DEPTH_STENCIL]->GetDepthStencil(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}