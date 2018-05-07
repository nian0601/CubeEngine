#include "stdafx.h"
#include "CE_MSDFFont.h"
#include "CE_Texture.h"

void CE_MSDFFont::LoadFromFile(const CE_String& aFilePath, CE_GPUContext& aContext)
{
	myMSDFGenerator.GenerateFont(aFilePath, myMaxHeight, myGlyphs);

	CE_String pngPath = CE_ReplaceExtension(aFilePath, "png");
	myTexture = new CE_Texture();
	myTexture->Load(pngPath, aContext);
}

bool CE_MSDFFont::GetGlyphData(char aCharacter, CE_MSDFGlyphData* aOutGlyph) const
{
	for (const CE_MSDFGlyphData& data : myGlyphs)
	{
		if (data.myID == aCharacter)
		{
			if(aOutGlyph)
				*aOutGlyph = data;

			return true;
		}
	}

	return false;
}