#include "stdafx.h"
#include "CE_MSDFFont.h"
#include "CE_Texture.h"

void CE_MSDFFont::LoadFromFile(const CE_String& aFilePath, CE_GPUContext& aContext)
{
	myMSDFGenerator.GenerateFont(aFilePath, myGlyphs);

	CE_String pngPath = CE_ReplaceExtension(aFilePath, "png");
	myTexture = new CE_Texture();
	myTexture->Load(pngPath, aContext);
}

const CE_MSDFGlyphData& CE_MSDFFont::GetGlyphData(char aCharacter) const
{
	for (const CE_MSDFGlyphData& data : myGlyphs)
	{
		if (data.myID == aCharacter)
			return data;
	}

	CE_ASSERT_ALWAYS("Failed to GetGlyphData from Font");
	return myEmptyGlyph;
}