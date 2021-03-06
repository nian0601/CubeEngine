#include "stdafx.h"
#include "CE_DirextXFactory.h"
#include <d3d11.h>

CE_DirextXFactory* CE_DirextXFactory::ourInstance = nullptr;

CE_DirextXFactory* CE_DirextXFactory::GetInstance()
{
	CE_ASSERT(ourInstance != nullptr, "You need to Create the DirectXFactory");
	return ourInstance;
}

void CE_DirextXFactory::Create(ID3D11Device* aDevice, ID3D11DeviceContext* aContext)
{
	CE_ASSERT(ourInstance == nullptr, "DirectXFactory was allready created");
	ourInstance = new CE_DirextXFactory(aDevice, aContext);
}

void CE_DirextXFactory::Destroy()
{
	CE_SAFE_DELETE(ourInstance);
}

void CE_DirextXFactory::CreateTexture2D(const CE_Vector2i& aSize, unsigned int aFormat, unsigned int someBindFlags, unsigned int aUsage, ID3D11Texture2D*& outTexture)
{
	D3D11_TEXTURE2D_DESC texDesc;
	CE_ZERO_MEMORY(texDesc);

	texDesc.Width = int(aSize.x);
	texDesc.Height = int(aSize.y);
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT(aFormat);
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE(aUsage);
	texDesc.BindFlags = someBindFlags;

	CE_ASSERT(SUCCEEDED(myDevice->CreateTexture2D(&texDesc, NULL, &outTexture)) == TRUE, "Failed to CreateTexture2D");
}

void CE_DirextXFactory::CreateDepthStencilView(unsigned int aFormat, unsigned int aViewDimension, ID3D11Texture2D* aTexture, ID3D11DepthStencilView*& outView)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc;
	CE_ZERO_MEMORY(stencilDesc);

	stencilDesc.Format = DXGI_FORMAT(aFormat);
	stencilDesc.ViewDimension = D3D11_DSV_DIMENSION(aViewDimension);

	stencilDesc.Texture2D.MipSlice = 0;

	CE_ASSERT(SUCCEEDED(myDevice->CreateDepthStencilView(aTexture, &stencilDesc, &outView)) == TRUE, "Failed to CreateDepthStencilView");
}

void CE_DirextXFactory::SetRasterizerState(CE_RasterizerState anState)
{
	myCurrentRasterizeState = anState;
	myContext->RSSetState(myRasterizerStates[anState]);
}

void CE_DirextXFactory::SetDepthStencilState(CE_DepthState anState)
{
	myCurrentDepthState = anState;
	myContext->OMSetDepthStencilState(myDepthStencilStates[anState], 1);
}

void CE_DirextXFactory::SetBlendState(CE_BlendState anState)
{
	myCurrentBlendState = anState;
	myContext->OMSetBlendState(myBlendStates[anState], NULL, 0xFFFFFFFF);
}

CE_DirextXFactory::CE_DirextXFactory(ID3D11Device* aDevice, ID3D11DeviceContext* aContext)
	: myDevice(aDevice)
	, myContext(aContext)
{
	SetupRasterizerStates();
	SetupDepthStencilStates();
	SetupBlendStates();
	SetupSamplerStates();

	myContext->PSSetSamplers(0, 1, &mySamplerStates[LINEAR_SAMPLING]);
	myContext->PSSetSamplers(1, 1, &mySamplerStates[LINEAR_CLAMP_SAMPLING]);
	myContext->PSSetSamplers(2, 1, &mySamplerStates[POINT_SAMPLING]);
	myContext->PSSetSamplers(3, 1, &mySamplerStates[POINT_WRAP_SAMPLING]);
}


CE_DirextXFactory::~CE_DirextXFactory()
{
	for (int i = 0; i < _RAZTER_COUNT; ++i)
		CE_SAFE_RELEASE(myRasterizerStates[i]);

	for (int i = 0; i < _DEPTH_COUNT; ++i)
		CE_SAFE_RELEASE(myDepthStencilStates[i]);

	for (int i = 0; i < _BLEND_COUNT; ++i)
		CE_SAFE_RELEASE(myBlendStates[i]);

	for (int i = 0; i < _SAMPLER_COUNT; ++i)
		CE_SAFE_RELEASE(mySamplerStates[i]);
}

void CE_DirextXFactory::SetupRasterizerStates()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.FrontCounterClockwise = false;
	desc.DepthBias = false;
	desc.DepthBiasClamp = 0;
	desc.SlopeScaledDepthBias = 0;
	desc.DepthClipEnable = false;
	desc.ScissorEnable = false;
	desc.MultisampleEnable = false;
	desc.AntialiasedLineEnable = false;


	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_BACK;
	myDevice->CreateRasterizerState(&desc, &myRasterizerStates[static_cast<int>(CE_RasterizerState::WIRE_FRAME)]);


	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	myDevice->CreateRasterizerState(&desc, &myRasterizerStates[static_cast<int>(CE_RasterizerState::CULL_BACK)]);

	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_NONE;
	myDevice->CreateRasterizerState(&desc, &myRasterizerStates[static_cast<int>(CE_RasterizerState::NO_CULLING)]);
}

void CE_DirextXFactory::SetupDepthStencilStates()
{
	D3D11_DEPTH_STENCIL_DESC  stencilDesc;
	stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	stencilDesc.StencilEnable = true;
	stencilDesc.StencilReadMask = 0xFF;
	stencilDesc.StencilWriteMask = 0xFF;
	stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	stencilDesc.DepthEnable = true;
	stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	HRESULT hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(CE_DepthState::ENABLED)]);
	CE_ASSERT(FAILED(hr) == false, "Failed to create ENABLED depthstate");

	stencilDesc.DepthEnable = false;
	stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(CE_DepthState::DISABLED)]);
	CE_ASSERT(FAILED(hr) == false, "Failed to create DISABLED depthstate");

	stencilDesc.DepthEnable = false;
	stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	stencilDesc.StencilEnable = false;
	stencilDesc.StencilReadMask = UINT8(0xFF);
	stencilDesc.StencilWriteMask = 0x0;
	hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(CE_DepthState::NO_READ_NO_WRITE)]);
	CE_ASSERT(FAILED(hr) == false, "Failed to create NO_READ_NO_WRITE depthstate");

	stencilDesc.DepthEnable = true;
	stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
	stencilDesc.StencilEnable = false;
	stencilDesc.StencilReadMask = UINT8(0xFF);
	stencilDesc.StencilWriteMask = 0x0;

	stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
	stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
	stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
	stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
	stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
	stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

	hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(CE_DepthState::READ_NO_WRITE)]);
	CE_ASSERT(FAILED(hr) == false, "Failed to create READ_NO_WRITE depthstate");
}

void CE_DirextXFactory::SetupBlendStates()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = true;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;


	HRESULT hr = myDevice->CreateBlendState(&blendDesc, &myBlendStates[static_cast<int>(CE_BlendState::ALPHA_BLEND)]);
	CE_ASSERT(FAILED(hr) == false, "Failed to CreateAlphaBlendState");

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	hr = myDevice->CreateBlendState(&blendDesc, &myBlendStates[static_cast<int>(CE_BlendState::NO_BLEND)]);
	CE_ASSERT(FAILED(hr) == false, "Failed to CreateNoAlphaBlendState");


	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	hr = myDevice->CreateBlendState(&blendDesc, &myBlendStates[static_cast<int>(CE_BlendState::LIGHT_BLEND)]);
	CE_ASSERT(FAILED(hr) == false, "Failed to CreateLightBlendState");
}

void CE_DirextXFactory::SetupSamplerStates()
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT result = myDevice->CreateSamplerState(&samplerDesc, &mySamplerStates[static_cast<int>(CE_SamplerState::LINEAR_SAMPLING)]);
	CE_ASSERT(FAILED(result) == false, "Failed to LinearSampling-state");

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

	result = myDevice->CreateSamplerState(&samplerDesc, &mySamplerStates[static_cast<int>(CE_SamplerState::POINT_WRAP_SAMPLING)]);
	CE_ASSERT(FAILED(result) == false, "Failed to PointWrapSampling-state");

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	result = myDevice->CreateSamplerState(&samplerDesc, &mySamplerStates[static_cast<int>(CE_SamplerState::POINT_SAMPLING)]);
	CE_ASSERT(FAILED(result) == false, "Failed to PointSampling-state");

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	result = myDevice->CreateSamplerState(&samplerDesc, &mySamplerStates[static_cast<int>(CE_SamplerState::LINEAR_CLAMP_SAMPLING)]);
	CE_ASSERT(FAILED(result) == false, "Failed to LinearClampSampling-state");
}
