#include "stdafx.h"
#include "CE_Texture.h"
#include "CE_GPUContext.h"

#include <WICTextureLoader.h>

CE_Texture::CE_Texture()
	: myShaderView(nullptr)
{
}


CE_Texture::~CE_Texture()
{
	CE_SAFE_RELEASE(myShaderView);
}

void CE_Texture::Load(const CE_String& aFilePath, CE_GPUContext& aGPUContext)
{
	myFilePath = aFilePath;

	const int fileLenght = strlen(aFilePath.c_str()) + 1;
	wchar_t* wCharFile = new wchar_t[fileLenght];
	size_t tempSize;
	mbstowcs_s(&tempSize, wCharFile, fileLenght, aFilePath.c_str(), fileLenght);

	

	HRESULT hr = DirectX::CreateWICTextureFromFile(aGPUContext.GetDevice(), wCharFile, nullptr, &myShaderView);
	CE_ASSERT(FAILED(hr) == false, "Failed to create texture");

	ID3D11Resource* resource = nullptr;
	myShaderView->GetResource(&resource);

	ID3D11Texture2D* tex2D = nullptr;
	hr = resource->QueryInterface(&tex2D);
	CE_ASSERT(FAILED(hr) == false, "Failed to get texture size");

	D3D11_TEXTURE2D_DESC desc;
	tex2D->GetDesc(&desc);
	mySize.x = static_cast<float>(desc.Width);
	mySize.y = static_cast<float>(desc.Height);

	CE_SAFE_DELETE_ARRAY(wCharFile);
}