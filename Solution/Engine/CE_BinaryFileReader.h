#pragma once
class CE_BinaryFileReader
{
public:
	CE_BinaryFileReader(const char* aFile);
	~CE_BinaryFileReader();

	bool IsOpen() const { return myStatus == 0; };

	template<typename T>
	void Read(T& someData);

	void Read(void* someData, int someDataLenght);
private:
	FILE* myFile;
	int myStatus;
	const char* myFilePath;
};

template<typename T>
void CE_BinaryFileReader::Read(T& someData)
{
	CE_ASSERT(myStatus == 0, "Tried to read from a file that wasnt properly opened!");
	fread(&someData, sizeof(someData), 1, myFile);
}

