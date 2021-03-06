#include "stdafx.h"

#include "CE_DirextXFactory.h"
#include "CE_GPUContext.h"
#include "CE_Texture.h"

#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

#include "stb_image.h"

CE_Texture::CE_Texture()
	: myTexture(nullptr)
	, myShaderView(nullptr)
	, myRenderTarget(nullptr)
	, myDepthTexture(nullptr)
	, myDepthShaderView(nullptr)
	, myDepthStencil(nullptr)
{
}


CE_Texture::~CE_Texture()
{
	CE_SAFE_RELEASE(myShaderView);
	CE_SAFE_RELEASE(myRenderTarget);
	CE_SAFE_RELEASE(myDepthShaderView);
	CE_SAFE_RELEASE(myDepthStencil);
}

void CE_Texture::InitForShader(const CE_Vector2i& aSize, unsigned int aBindFlag, unsigned int aFormat, CE_GPUContext& aGPUContext)
{
	myFilePath = "InitForShader";
	myFormat = aFormat;
	mySize = aSize;

	if ((aBindFlag & D3D11_BIND_SHADER_RESOURCE) > 0 || (aBindFlag & D3D11_BIND_RENDER_TARGET) > 0)
		CreateRenderTarget(aSize, aBindFlag, aFormat, aGPUContext);

	if ((aBindFlag & D3D11_BIND_DEPTH_STENCIL) > 0)
		CreateDepthStencil(aSize, aGPUContext);
}

void CE_Texture::InitAsBackBuffer(const CE_Vector2i& aSize, ID3D11Texture2D* aTexture, CE_GPUContext& aGPUContext)
{
	myFilePath = "InitAsBackBuffer";
	mySize = aSize;

	ID3D11Device* device = aGPUContext.GetDevice();
	device->CreateRenderTargetView(aTexture, NULL, &myRenderTarget);
	//aDirectX->GetDevice()->CreateShaderResourceView(myBackbuffer, NULL, &myBackbufferShaderResourceView);

	CE_DirextXFactory::GetInstance()->CreateTexture2D(aSize, DXGI_FORMAT_D32_FLOAT, D3D11_BIND_DEPTH_STENCIL, 0, myDepthTexture);
	CE_DirextXFactory::GetInstance()->CreateDepthStencilView(DXGI_FORMAT_D32_FLOAT, D3D11_DSV_DIMENSION_TEXTURE2D, myDepthTexture, myDepthStencil);
}

void CE_Texture::Load(const CE_String& aFilePath, CE_GPUContext& /*aGPUContext*/)
{
	myFilePath = aFilePath;

	int nrChannels;
	unsigned char* data = stbi_load(aFilePath.c_str(), &mySize.x, &mySize.y, &nrChannels, 4);
	CE_ASSERT(data != nullptr, "Failed to load %s", aFilePath.c_str());

	Load(data, mySize, false);

	stbi_image_free(data);
}

void CE_Texture::Load(unsigned char* someData, const CE_Vector2i& aSize, bool aIsSingleChannel)
{
	mySize = aSize;

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = mySize.x;
	textureDesc.Height = mySize.y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;

	if(aIsSingleChannel)
		textureDesc.Format = DXGI_FORMAT_R8_UNORM;
	else 
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA textureData = {};
	textureData.pSysMem = someData;

	if(aIsSingleChannel)
		textureData.SysMemPitch = sizeof(unsigned char) * mySize.x;
	else
		textureData.SysMemPitch = sizeof(unsigned char) * 4 * mySize.x;

	textureData.SysMemSlicePitch = 0;

	
	CE_Engine::GetGPUContext().GetDevice()->CreateTexture2D(
		&textureDesc,
		&textureData,
		&myTexture);

	CE_Engine::GetGPUContext().GetDevice()->CreateShaderResourceView(
		myTexture,
		NULL,
		&myShaderView);
}

void CE_Texture::LoadDDS(const CE_String& aFilePath, CE_GPUContext& aGPUContext)
{
	myFilePath = aFilePath;

	const int fileLenght = strlen(aFilePath.c_str()) + 1;
	wchar_t* wCharFile = new wchar_t[fileLenght];
	size_t tempSize;
	mbstowcs_s(&tempSize, wCharFile, fileLenght, aFilePath.c_str(), fileLenght);


	HRESULT hr = DirectX::CreateDDSTextureFromFileEx(aGPUContext.GetDevice(), wCharFile, 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, D3D11_RESOURCE_MISC_TEXTURECUBE, false, nullptr, &myShaderView);
	CE_ASSERT(FAILED(hr) == false, "Failed to create texture");

	ID3D11Resource* resource = nullptr;
	myShaderView->GetResource(&resource);

	ID3D11Texture2D* tex2D = nullptr;
	hr = resource->QueryInterface(&tex2D);
	CE_ASSERT(FAILED(hr) == false, "Failed to get texture size");

	D3D11_TEXTURE2D_DESC desc;
	tex2D->GetDesc(&desc);
	mySize.x = desc.Width;
	mySize.y = desc.Height;

	CE_SAFE_DELETE_ARRAY(wCharFile);
}

void CE_Texture::CreateRenderTarget(const CE_Vector2i& aSize, unsigned int aBindFlag, unsigned int aFormat, CE_GPUContext& aGPUContext)
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = aSize.x;
	texDesc.Height = aSize.y;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = static_cast<DXGI_FORMAT>(aFormat);
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Device* device = aGPUContext.GetDevice();
	CE_ASSERT(SUCCEEDED(device->CreateTexture2D(&texDesc, NULL, &myTexture)), "Failed to Create texture");

	if ((aBindFlag & D3D11_BIND_SHADER_RESOURCE) > 0)
		CE_ASSERT(SUCCEEDED(device->CreateShaderResourceView(myTexture, NULL, &myShaderView)), "Failed to create shaderresource");

	if((aBindFlag & D3D11_BIND_RENDER_TARGET) > 0)
		CE_ASSERT(SUCCEEDED(device->CreateRenderTargetView(myTexture, NULL, &myRenderTarget)), "Failed to create rendertarget");
}

void CE_Texture::CreateDepthStencil(const CE_Vector2i& aSize, CE_GPUContext& aGPUContext)
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = aSize.x;
	texDesc.Height = aSize.y;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Device* device = aGPUContext.GetDevice();
	CE_ASSERT(SUCCEEDED(device->CreateTexture2D(&texDesc, NULL, &myDepthTexture)), "Failed to Create texture");

	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
	CE_ZERO_MEMORY(depthDesc);
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthDesc.Texture2D.MipSlice = 0;

	CE_ASSERT(SUCCEEDED(device->CreateDepthStencilView(myDepthTexture, &depthDesc, &myDepthStencil)), "Failed to create rendertarget");

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	CE_ZERO_MEMORY(viewDesc);
	viewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;
	viewDesc.Texture2D.MostDetailedMip = 0;
	CE_ASSERT(SUCCEEDED(device->CreateShaderResourceView(myDepthTexture, &viewDesc, &myDepthShaderView)), "Failed to create shaderresource");
}
