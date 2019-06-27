#pragma once

#include "stb_truetype.h"

class CE_Texture;

struct CE_GlyphData
{
	CE_Texture* myTexture;
	int myAdvance;
	CE_Vector2i mySize;
	CE_Vector4i myBoundingBox;
	bool myIsEmptyGlyph;
};

struct stbtt_fontinfo;

class CE_Font
{
public:
	CE_Font(const char* aFontPath, float aFontHeight);
	~CE_Font();

	const CE_GlyphData& GetGlyphData(const char aGlyph);
	int GetLineHeight() const { return myVerticalLineAdvance; }

	CE_Vector2f GetSize(const CE_String& aString);

private:
	CE_Map<char, CE_GlyphData> myGlyphs;

	stbtt_fontinfo* myFontInfo;
	float myScaleFactor;
	int myVerticalLineAdvance;
};
