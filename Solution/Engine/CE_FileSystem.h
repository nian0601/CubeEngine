#pragma once
#include "CE_GrowingArray.h"
#include <windows.h>

namespace CE_FileSystem
{
	struct FileInfo
	{
		CE_String myFileName;
		CE_String myFileNameNoExtention;
		CE_String myFilePath;
		unsigned long myLastTimeModifiedLowbit;
		unsigned long myLastTimeModifiedHighbit;
	};

	// The Contents of the file will be freed when the FileContent
	// goes out of scope, be careful if you allocated temporary FileContents
	struct FileContent
	{
		FileContent(bool aAutoFreeContents = true) : myContents(nullptr), myAutoFreeContents(aAutoFreeContents) {}
		~FileContent() { if(myAutoFreeContents) CE_SAFE_DELETE(myContents); }
		const unsigned char* myContents;
		long myFileSize;
		bool myAutoFreeContents;
	};

	bool GetAllFilesFromDirectory(const char* aDirectory, CE_GrowingArray<FileInfo>& someOutFilePaths);
	void GetFileName(const CE_String& aFilePath, CE_String& aNameOut);
	void GetFileExtention(const CE_String& aFilePath, CE_String& aExtentionOut);
	void RemoveFileExtention(const CE_String& aFilePath, CE_String& aNameOut);
	bool GetFileInfo(const CE_String& aFilePath, FileInfo& aFileInfoOut);

	bool UpdateFileInfo(CE_GrowingArray<FileInfo>& someFiles);

	void ReadEntireFile(const CE_String& aFilePath, FileContent& aFileContentOut);
}