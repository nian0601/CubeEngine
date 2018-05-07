#pragma once

#include "CE_MSDFGenerator.h"

class CE_GPUContext;
class CE_Texture;
class CE_MSDFFont
{
public:
	void LoadFromFile(const CE_String& aFilePath, CE_GPUContext& aContext);

	bool GetGlyphData(char aCharacter, CE_MSDFGlyphData* aOutGlyph) const;
	CE_Texture* GetTexture() { return myTexture; }
	float GetMaxHeight() const { return myMaxHeight; }

private:
	CE_GrowingArray<CE_MSDFGlyphData> myGlyphs;
	CE_MSDFGlyphData myEmptyGlyph;

	CE_Texture* myTexture;
	CE_MSDFGenerator myMSDFGenerator;
	float myMaxHeight;
};

