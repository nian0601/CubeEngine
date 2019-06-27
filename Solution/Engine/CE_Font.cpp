#include "stdafx.h"

#include "CE_Font.h"
#include "CE_FileSystem.h"
#include "CE_Texture.h"

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include "stb_truetype.h"

CE_Font::CE_Font(const char* aFontPath, float aFontHeight)
{
	CE_FileSystem::FileContent file(false);
	CE_FileSystem::ReadEntireFile(aFontPath, file);

	myFontInfo = new stbtt_fontinfo();
	stbtt_InitFont(myFontInfo, file.myContents, stbtt_GetFontOffsetForIndex(file.myContents, 0));

	myScaleFactor = stbtt_ScaleForPixelHeight(myFontInfo, aFontHeight);

	int ascent;
	int descent;
	int lineGap;
	stbtt_GetFontVMetrics(myFontInfo, &ascent, &descent, &lineGap);
	int lineHeight = ascent - descent + lineGap;

	myVerticalLineAdvance = static_cast<int>((float)lineHeight * myScaleFactor);
}

CE_Font::~CE_Font()
{
	CE_SAFE_DELETE(myFontInfo);
}

const CE_GlyphData& CE_Font::GetGlyphData(const char aGlyph)
{
	if (const CE_GlyphData* data = myGlyphs.GetIfExists(aGlyph))
		return *data;

	CE_GlyphData& newData = myGlyphs[aGlyph];
	if (aGlyph == ' ')
	{
		newData.myAdvance = 5;
		newData.mySize.x = 5;
		return newData;
	}

	CE_Vector2i size;
	CE_Vector2i offsets;
	unsigned char* bitmap = stbtt_GetCodepointBitmap(
		myFontInfo,
		0,
		myScaleFactor,
		aGlyph,
		&size.x,
		&size.y,
		&offsets.x,
		&offsets.y);

	CE_Vector4i boundingBox;
	stbtt_GetCodepointBox(myFontInfo, aGlyph, &boundingBox.x, &boundingBox.y, &boundingBox.z, &boundingBox.w);

	int advance;
	stbtt_GetCodepointHMetrics(myFontInfo, aGlyph, &advance, nullptr);

	newData.myTexture = new CE_Texture();
	newData.myTexture->Load(bitmap, size, true);
	stbtt_FreeBitmap(bitmap, nullptr);

	newData.myAdvance = static_cast<int>((float)advance * myScaleFactor);
	newData.myBoundingBox.x = static_cast<int>((float)boundingBox.x * myScaleFactor);
	newData.myBoundingBox.y = static_cast<int>((float)boundingBox.y * myScaleFactor);
	newData.myBoundingBox.z = static_cast<int>((float)boundingBox.z * myScaleFactor);
	newData.myBoundingBox.w = static_cast<int>((float)boundingBox.w * myScaleFactor);
	newData.mySize.x = newData.myBoundingBox.z - newData.myBoundingBox.x;
	newData.mySize.y = newData.myBoundingBox.w - newData.myBoundingBox.y;
	newData.myIsEmptyGlyph = false;

	return newData;
}

CE_Vector2f CE_Font::GetSize(const CE_String& aString)
{
	CE_Vector2f size;

	for (int i = 0; i < aString.Lenght() + 1; ++i)
	{
		const CE_GlyphData& data = GetGlyphData(aString[i]);
		//size.x += data.mySize.x;
		size.x += data.myAdvance;
	}

	size.y = static_cast<float>(GetLineHeight());
	return size;
}
