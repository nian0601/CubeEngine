#include "stdafx.h"
#include "CE_DirextXFactory.h"
#include <d3d11.h>

CE_DirextXFactory* CE_DirextXFactory::ourInstance = nullptr;

CE_DirextXFactory* CE_DirextXFactory::GetInstance()
{
	CE_ASSERT(ourInstance != nullptr, "You need to Create the DirectXFactory");
	return ourInstance;
}

void CE_DirextXFactory::Create(ID3D11Device* aDevice)
{
	CE_ASSERT(ourInstance == nullptr, "DirectXFactory was allready created");
	ourInstance = new CE_DirextXFactory(aDevice);
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

CE_DirextXFactory::CE_DirextXFactory(ID3D11Device* aDevice)
	: myDevice(aDevice)
{
}


CE_DirextXFactory::~CE_DirextXFactory()
{
}
