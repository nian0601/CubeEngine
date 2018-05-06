#include "stdafx.h"
#include "CE_BinaryFileWriter.h"


CE_BinaryFileWriter::CE_BinaryFileWriter(const char* aFile)
{
	int result = fopen_s(&myFile, aFile, "wb");
	CE_ASSERT(result == 0, "Failed to open file %s", aFile);
}


CE_BinaryFileWriter::~CE_BinaryFileWriter()
{
	fclose(myFile);
}

void CE_BinaryFileWriter::Write(const void* someData, int aDataLenght)
{
	fwrite(someData, aDataLenght, 1, myFile);
}
