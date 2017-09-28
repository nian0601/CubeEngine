#pragma once

struct CE_CharData
{
	void LoadFromLine(const CE_String& aLine, const CE_Vector2f& aTextureSize);
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

class CE_Font
{
public:
	CE_Font();

	void LoadFromFile(const CE_String& aFilePath);

	const CE_CharData& GetCharData(char aCharacter) const;
	//Texture* GetTexture() { return myTexture; }

	float GetScale() const { return myScale; }
	void SetScale(float aScale) { myScale = aScale; }

	float GetMaxHeight() const { return myMaxHeight * GetScale(); }

private:

	CE_Map<char, CE_CharData> myCharacters;
	CE_CharData myEmptyCharData;
	//Texture* myTexture;
	float myScale;
	float myMaxHeight;
};

