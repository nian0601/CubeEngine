#pragma once
#include "CE_GrowingArray.h"
#include <windows.h>

namespace CE_FileSystem
{
	struct FileInfo
	{
		CE_String myFileName;
		CE_String myFilePath;
		unsigned long myLastTimeModifiedLowbit;
		unsigned long myLastTimeModifiedHighbit;
	};

	bool GetAllFilesFromDirectory(const char* aDirectory, CE_GrowingArray<FileInfo>& someOutFilePaths);
	void GetFileName(const CE_String& aFilePath, CE_String& aNameOut);
	void GetFileExtention(const CE_String& aFilePath, CE_String& aExtentionOut);
	bool GetFileInfo(const CE_String& aFilePath, FileInfo& aFileInfoOut);

	bool UpdateFileInfo(CE_GrowingArray<FileInfo>& someFiles);
}