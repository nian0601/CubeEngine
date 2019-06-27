#include "stdafx.h"
#include "CE_TextureManager.h"
#include "CE_Texture.h"

CE_TextureManager* CE_TextureManager::ourInstance = nullptr;
void CE_TextureManager::Create(CE_GPUContext& aGPUContext)
{
	CE_ASSERT(ourInstance == nullptr, "Tried to create TextureManager twice!");
	ourInstance = new CE_TextureManager(aGPUContext);
}

void CE_TextureManager::Destroy()
{
	CE_ASSERT(ourInstance != nullptr, "Tried to delete TextureManager twice!");
	CE_SAFE_DELETE(ourInstance);

}

CE_TextureManager::CE_TextureManager(CE_GPUContext& aGPUContext)
	: CE_AssetManager("Data/Textures", false)
	, myGPUContext(aGPUContext)
{
	myEmptyTexture = new CE_Texture();
	myEmptyTexture->Load("Data/UI/empty_white.png", myGPUContext);
}

const CE_Texture* CE_TextureManager::GetUITexture(CUI_Theme aTheme, const char* aTextureName)
{
	if (const CE_Texture* const* texture = myUITextures.GetIfExists(aTextureName))
		return *texture;

	CE_String filePath;
	filePath = "Data/UI/";

	switch (aTheme)
	{
	case THEME_BLUE:
		filePath += "blue/";
		break;
	case THEME_GRAY:
		filePath += "gray/";
		break;
	case THEME_GREEN:
		filePath += "green/";
		break;
	case THEME_RED:
		filePath += "red/";
		break;
	case THEME_YELLOW:
		filePath += "yellow/";
		break;
	default:
		CE_ASSERT(false, "Unhandled UITheme");
		break;
	}

	filePath += aTextureName;
	filePath += ".png";

	CE_Texture* texture = new CE_Texture();
	texture->Load(filePath, myGPUContext);
	myUITextures[aTextureName] = texture;
	return texture;
}
