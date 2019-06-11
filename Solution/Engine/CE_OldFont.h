#pragma once

#include "CE_Map.h"

struct CE_CharData
{
	void LoadFromLine(const CE_String& aLine, const CE_Vector2i& aTextureSize);
	CE_String LoadToken(const CE_String& aLine, const CE_String& aToken);

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

class CE_GPUContext;
class CE_Texture;

class CE_OldFont
{
public:
	CE_OldFont();

	void LoadFromFile(const CE_String& aFilePath, CE_GPUContext& aContext);

	bool GetCharData(char aCharacter, CE_CharData* aOutData = nullptr) const;
	CE_Texture* GetTexture() { return myTexture; }

	float GetScale() const { return myScale; }
	void SetScale(float aScale) { myScale = aScale; }

	float GetMaxHeight() const { return myMaxHeight * GetScale(); }
	CE_Vector2f GetSize(const CE_String& aString) const;

private:

	CE_Map<char, CE_CharData> myCharacters;
	CE_CharData myEmptyCharData;
	CE_Texture* myTexture;
	float myScale;
	float myMaxHeight;
};

