#pragma once
#include "CE_AssetManager.h"
#include "CUI_Defines.h"

class CE_GPUContext;
class CE_Texture;
class CE_TextureManager : public CE_AssetManager
{
public:
	static void Create(CE_GPUContext& aGPUContext);
	static void Destroy();
	static CE_TextureManager * GetInstance() { return ourInstance; }

	const CE_Texture* GetUITexture(CUI_Theme aTheme, const char* aTextureName);
	const CE_Texture* GetEmptyTexture() const { return myEmptyTexture; }

private:
	CE_TextureManager(CE_GPUContext& aGPUContext);

	CE_GPUContext& myGPUContext;
	CE_Map<CE_String, CE_Texture*> myUITextures;
	CE_Texture* myEmptyTexture;

	static CE_TextureManager* ourInstance;
};