#include "stdafx.h"
#include "CE_BinaryFileWriter.h"


CE_BinaryFileWriter::CE_BinaryFileWriter(const char* aFile)
	: myFilePath(aFile)
{
	myStatus = fopen_s(&myFile, aFile, "wb");
}


CE_BinaryFileWriter::~CE_BinaryFileWriter()
{
	if (IsOpen())
		fclose(myFile);
}

void CE_BinaryFileWriter::Write(const void* someData, int aDataLenght)
{
	CE_ASSERT(myStatus == 0, "Tried to write to a file that wasnt properly opened!");
	fwrite(someData, aDataLenght, 1, myFile);
}
