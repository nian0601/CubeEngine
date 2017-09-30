#pragma once

#include "CE_MSDFGenerator.h"

class CE_GPUContext;
class CE_Texture;
class CE_MSDFFont
{
public:
	void LoadFromFile(const CE_String& aFilePath, CE_GPUContext& aContext);

	const CE_MSDFGlyphData& GetGlyphData(char aCharacter) const;
	CE_Texture* GetTexture() { return myTexture; }
	
private:
	CE_GrowingArray<CE_MSDFGlyphData> myGlyphs;
	CE_MSDFGlyphData myEmptyGlyph;

	CE_Texture* myTexture;
	CE_MSDFGenerator myMSDFGenerator;
};

