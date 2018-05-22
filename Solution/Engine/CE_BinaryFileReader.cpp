#include "stdafx.h"
#include "CE_BinaryFileReader.h"


CE_BinaryFileReader::CE_BinaryFileReader(const char* aFile)
	: myFilePath(aFile)
{
	myStatus = fopen_s(&myFile, aFile, "rb");
}


CE_BinaryFileReader::~CE_BinaryFileReader()
{
	if(IsOpen())
		fclose(myFile);
}

void CE_BinaryFileReader::Read(void* someData, int someDataLenght)
{
	CE_ASSERT(myStatus == 0, "Tried to read from a file that wasnt properly opened!");
	fread(someData, someDataLenght, 1, myFile);
}
