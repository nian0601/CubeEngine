#include "stdafx.h"
#include "CE_MSDFGenerator.h"

#include <msdfgen.h>
#include <msdfgen-ext.h>
using namespace msdfgen;

namespace CE_MSDFGenerator_priv
{
	const int locNumChars = 128;
	const int locGlyphsPerRow = static_cast<int>(sqrt(locNumChars) + 0.5f);
	const int locGlyphSize = 32;
	const int locSheetSize = locGlyphSize * locGlyphsPerRow;

	CE_MSDFGlyphData LoadGlyph(FontHandle* aFont, char aGlyph, Bitmap<FloatRGB>& aGlyphSheet, int aSheetX, int aSheetY)
	{
		CE_MSDFGlyphData data;

		Shape shape;
		double advance;
		if (loadGlyph(shape, aFont, aGlyph, &advance))
		{
			shape.normalize();
			edgeColoringSimple(shape, 3.0);

			int glyphSize = CE_MSDFGenerator_priv::locGlyphSize;
			Bitmap<FloatRGB> msdf(glyphSize, glyphSize);
			generateMSDF(msdf, shape, 4.0, 1.0, Vector2(4.0, 4.0));

			for (int y = 0; y < glyphSize; ++y)
			{
				for (int x = 0; x < glyphSize; ++x)
				{
					FloatRGB& pixel = aGlyphSheet(aSheetX + x, aSheetY + y);
					pixel = msdf(x, y);
				}
			}


			double left = 0.0;
			double bottom = 0.0;
			double right = 0.0;
			double top = 0.0;
			shape.bounds(left, bottom, right, top);

			data.myHeight = static_cast<int>(top + 0.5);
			data.myWidth = static_cast<int>(right + 0.5);
			data.myXAdvance = static_cast<float>(advance);

			data.myTopLeftUV.x = static_cast<float>(aSheetX) / locSheetSize;
			data.myTopLeftUV.y = static_cast<float>(aSheetY) / locSheetSize;

			data.myBottomRightUV.x = static_cast<float>(aSheetX + locGlyphSize) / locSheetSize;
			data.myBottomRightUV.y = static_cast<float>(aSheetY + locGlyphSize) / locSheetSize;

			data.myID = aGlyph;
			
		}

		return data;
	}
}

CE_MSDFGenerator::CE_MSDFGenerator()
{
}


CE_MSDFGenerator::~CE_MSDFGenerator()
{
}

const char anArray[] =
{
	//'A',
	//'B',
	//'C',
	//'D',
	//'E',
	65,
	66,
	67,
	68,
	69,
	70,
};

void CE_MSDFGenerator::GenerateFont(const CE_String& aFontPath, CE_GrowingArray<CE_MSDFGlyphData>& outGlyphData)
{
	FreetypeHandle* ft = initializeFreetype();
	CE_ASSERT(ft != nullptr, "Failed to initialize FreeType");

	FontHandle* font = loadFont(ft, aFontPath.c_str());
	CE_ASSERT(font != nullptr, "Failed to Load Font: %s", aFontPath.c_str());
	
	int glyphSize = CE_MSDFGenerator_priv::locGlyphSize;
	int glyphsPerRow = CE_MSDFGenerator_priv::locGlyphsPerRow;
	int sheetSize = CE_MSDFGenerator_priv::locSheetSize;

	Bitmap<FloatRGB> glyphSheet(sheetSize, sheetSize);

	char index = 0;
	for (int y = 0; y < glyphsPerRow; ++y)
	{
		for (int x = 0; x < glyphsPerRow; ++x)
		{
			int sheetX = y * glyphSize;
			int sheetY = x * glyphSize;

			//if (index >= sizeof(anArray) / sizeof(char))
			//	outGlyphData.Add(CE_MSDFGenerator_priv::LoadGlyph(font, index, glyphSheet, sheetX, sheetY));
			//else
			//	outGlyphData.Add(CE_MSDFGenerator_priv::LoadGlyph(font, anArray[index], glyphSheet, sheetX, sheetY));

			outGlyphData.Add(CE_MSDFGenerator_priv::LoadGlyph(font, 'B', glyphSheet, sheetX, sheetY));

			//outGlyphData[index] = CE_MSDFGenerator_priv::LoadGlyph(font, index, glyphSheet, sheetX, sheetY);
			//outGlyphData[65] = CE_MSDFGenerator_priv::LoadGlyph(font, 65, glyphSheet, sheetX, sheetY);

			//outGlyphData['A'] = CE_MSDFGenerator_priv::LoadGlyph(font, 'A', glyphSheet, sheetX, sheetY);


			++index;
		}
	}

	destroyFont(font);
	deinitializeFreetype(ft);

	CE_String pngPath = CE_ReplaceExtension(aFontPath, "png");
	savePng(glyphSheet, pngPath.c_str());
}
