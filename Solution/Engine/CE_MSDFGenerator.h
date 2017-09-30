#pragma once

struct CE_MSDFGlyphData
{
	char myID;
	int myX;
	int myY;
	int myWidth;
	int myHeight;
	float myXOffset;
	float myYOffset;
	float myXAdvance;

	CE_Vector2f myTopLeftUV;
	CE_Vector2f myBottomRightUV;
};

class CE_MSDFGenerator
{
public:
	CE_MSDFGenerator();
	~CE_MSDFGenerator();

	void GenerateFont(const CE_String& aFontPath, CE_GrowingArray<CE_MSDFGlyphData>& ourGlyphData);
};

