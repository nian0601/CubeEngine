#pragma once
class CE_BinaryFileReader
{
public:
	CE_BinaryFileReader(const char* aFile);
	~CE_BinaryFileReader();

	template<typename T>
	void Read(T& someData);

	void Read(void* someData, int someDataLenght);
private:
	FILE* myFile;
};

template<typename T>
void CE_BinaryFileReader::Read(T& someData)
{
	fread(&someData, sizeof(someData), 1, myFile);
}

