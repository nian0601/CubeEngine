#include "stdafx.h"
#include "CE_Font.h"

#include <istream>
#include <string>
#include "CE_Texture.h"

void CE_CharData::LoadFromLine(const CE_String& aLine, const CE_Vector2f& aTextureSize)
{
	CE_String id = LoadToken(aLine, "id=");
	CE_String x = LoadToken(aLine, "x=");
	CE_String y = LoadToken(aLine, "y=");
	CE_String width = LoadToken(aLine, "width=");
	CE_String height = LoadToken(aLine, "height=");
	CE_String xoffset = LoadToken(aLine, "xoffset=");
	CE_String yoffset = LoadToken(aLine, "yoffset=");
	CE_String xadvance = LoadToken(aLine, "xadvance=");

	myID = static_cast<char>(atoi(id.c_str()));
	myX = atoi(x.c_str());
	myY = atoi(y.c_str());
	myWidth = atoi(width.c_str());
	myHeight = atoi(height.c_str());
	myXOffset = static_cast<float>(atof(xoffset.c_str()));
	myYOffset = static_cast<float>(atof(yoffset.c_str()));
	myXAdvance = static_cast<float>(atof(xadvance.c_str()));

	myTopLeftUV.x = static_cast<float>(myX);
	myTopLeftUV.y = static_cast<float>(myY);
	myBottomRightUV.x = myTopLeftUV.x + myWidth;
	myBottomRightUV.y = myTopLeftUV.y + myHeight;

	myTopLeftUV /= aTextureSize;
	myBottomRightUV /= aTextureSize;
}

CE_String CE_CharData::LoadToken(const CE_String& aLine, const CE_String& aToken)
{
	int startIndex = aLine.Find(aToken);
	startIndex += aToken.Lenght()+1;
	int secondIndex = startIndex;
	while (aLine[secondIndex] != ' ')
		++secondIndex;

	return aLine.SubStr(startIndex, secondIndex);
}


CE_Font::CE_Font()
	: myScale(0.33f)
{
}

void CE_Font::LoadFromFile(const CE_String& aFilePath, CE_GPUContext& aContext)
{
	CE_String texturePath = aFilePath;

	CE_String txtPath = CE_ReplaceExtension(aFilePath, "txt");

	myTexture = new CE_Texture();
	myTexture->Load(aFilePath, aContext);

	std::ifstream stream;
	stream.open(txtPath.c_str());
	if (!stream.is_open())
		return;

	std::string stdLine;
	std::getline(stream, stdLine); //first line is just the name of the font, dont care
	std::getline(stream, stdLine); //the second like is the number of characers in the file ("chars count=191"), dont care

	CE_Vector2<float> textureSize = myTexture->GetSize();
	while (std::getline(stream, stdLine))
	{
		CE_CharData data;
		data.LoadFromLine(stdLine.c_str(), textureSize);
		myCharacters[data.myID] = data;

		if (data.myHeight > myMaxHeight)
			myMaxHeight = static_cast<float>(data.myHeight);
	}
}

const CE_CharData& CE_Font::GetCharData(char aCharacter) const
{
	if (myCharacters.KeyExists(aCharacter))
		return myCharacters.Get(aCharacter);

	CE_ASSERT_ALWAYS("Failed to GetCharData from Font");
	return myEmptyCharData;
}

CE_Vector2f CE_Font::GetSize(const CE_String& aString) const
{
	CE_Vector2f size;

	for (int i = 0; i < aString.Lenght(); ++i)
	{
		const CE_CharData& data = GetCharData(aString[i]);
		size.x += data.myXAdvance * myScale;
	}

	size.y = GetMaxHeight();
	return size;
}
