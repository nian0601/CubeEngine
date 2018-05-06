#pragma once
class CE_BinaryFileWriter
{
public:
	CE_BinaryFileWriter(const char* aFile);
	~CE_BinaryFileWriter();

	template<typename T>
	void Write(const T& someData);

	void Write(const void* someData, int aDataLenght);

private:
	FILE* myFile;
};

template<typename T>
void CE_BinaryFileWriter::Write(const T& someData)
{
	fwrite(&someData, sizeof(someData), 1, myFile);
}