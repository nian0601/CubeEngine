#include "stdafx.h"
#include "CE_BinaryFileReader.h"


CE_BinaryFileReader::CE_BinaryFileReader(const char* aFile)
{
	int result = fopen_s(&myFile, aFile, "rb");
	CE_ASSERT(result == 0, "Failed to open file %s", aFile);
}


CE_BinaryFileReader::~CE_BinaryFileReader()
{
	fclose(myFile);
}

void CE_BinaryFileReader::Read(void* someData, int someDataLenght)
{
	fread(someData, someDataLenght, 1, myFile);
}
