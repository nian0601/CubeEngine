#pragma once

class CE_Any;

class CE_BinaryFileWriter
{
public:
	CE_BinaryFileWriter(const char* aFile);
	~CE_BinaryFileWriter();

	bool IsOpen() const { return myStatus == 0; };

	template<typename T>
	void Write(const T& someData);
	void Write(const CE_Any& someData);

	void Write(const void* someData, int aDataLenght);

private:
	FILE* myFile;
	int myStatus;
	const char* myFilePath;
};

template<typename T>
void CE_BinaryFileWriter::Write(const T& someData)
{
	CE_ASSERT(myStatus == 0, "Tried to write to a file that wasnt properly opened!");
	fwrite(&someData, sizeof(someData), 1, myFile);
}