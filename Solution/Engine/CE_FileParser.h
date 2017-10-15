#pragma once

class CE_FileParser
{
public:
	CE_FileParser(const char* aFile);
	~CE_FileParser();

	bool ReadLine(CE_String& outLine);
	void SplitLine(const CE_String& aLine, CE_GrowingArray<CE_String>& outWords);

	void TrimBeginAndEnd(CE_String& aString);

	float GetFloat(CE_String& aWord) const;
	int GetInt(CE_String& aWord) const;

private:
	FILE* myFile;
};

