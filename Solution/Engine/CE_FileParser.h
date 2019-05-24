#pragma once

class CE_FileParser
{
public:
	CE_FileParser(const char* aFile);
	~CE_FileParser();

	bool ReadLine(CE_String& outLine);

	void TrimBeginAndEnd(CE_String& aLine) const;
	void SplitLine(const CE_String& aLine, CE_GrowingArray<CE_String>& outWords) const;

	void SplitWord(const CE_String& aLine, const char* aSeperator, CE_GrowingArray<CE_String>& outWords) const;

	float GetFloat(const CE_String& aWord) const;
	int GetInt(const CE_String& aWord) const;

private:
	FILE* myFile;
};

