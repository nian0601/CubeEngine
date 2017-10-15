#include "stdafx.h"
#include "CE_FileParser.h"

CE_FileParser::CE_FileParser(const char* aFile)
{
	int result = fopen_s(&myFile, aFile, "r");
	CE_ASSERT(result == 0, "Failed to open file %s", aFile);
}

CE_FileParser::~CE_FileParser()
{
	fclose(myFile);
}

bool CE_FileParser::ReadLine(CE_String& outLine)
{
	char text[64];
	if (fgets(text, 64, myFile) == NULL)
		return false;

	outLine = text;
	return true;
}

void CE_FileParser::SplitLine(const CE_String& aLine, CE_GrowingArray<CE_String>& outWords)
{
	outWords.RemoveAll();

	int begin = 0;
	int end = aLine.Find(" ", begin);
	if (end == -1)
	{
		outWords.Add(aLine);
		return;
	}

	while (end != -1)
	{
		outWords.Add(aLine.SubStr(begin, end-1));

		begin = end+1;
		end = aLine.Find(" ", begin);
	}

	outWords.Add(aLine.SubStr(begin, aLine.Lenght()));
}

void CE_FileParser::TrimBeginAndEnd(CE_String& aString)
{
	if (aString.Empty() == true)
		return;

	int begin = 0;
	while (aString[begin] == ' ' || aString[begin] == '\t' || aString[begin] == '\n')
		++begin;

	int end = aString.Lenght();
	if (end > begin)
	{
		while (aString[end] == ' ' || aString[end] == '\t' || aString[end] == '\n')
			--end;
	}

	if (begin != 0 || end != aString.Lenght())
		aString = aString.SubStr(begin, end);
}

float CE_FileParser::GetFloat(CE_String& aWord) const
{
	return static_cast<float>(atof(aWord.c_str()));
}

int CE_FileParser::GetInt(CE_String& aWord) const
{
	return static_cast<int>(atoll(aWord.c_str()));
}
