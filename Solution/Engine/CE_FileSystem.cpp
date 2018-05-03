#include "stdafx.h"
#include "CE_FileSystem.h"

bool CE_FileSystem::GetAllFilesFromDirectory(const char* aDirectory, CE_GrowingArray<FileInfo>& someOutFilePaths)
{
	CE_ASSERT(strlen(aDirectory) + 3 < MAX_PATH, "Path to directory is too long");

	CE_String directory = aDirectory;
	directory += "/*";

	WIN32_FIND_DATA data;
	HANDLE filehandle = FindFirstFile(directory.c_str(), &data);

	if (filehandle == INVALID_HANDLE_VALUE)
		return false;

	do
	{
		CE_String name = data.cFileName;
		if (name == "." || name == "..")
			continue;

		CE_String fullPath = aDirectory;
		fullPath += "/";
		fullPath += name;

		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			GetAllFilesFromDirectory(fullPath.c_str(), someOutFilePaths);
		}
		else
		{
			FileInfo& info = someOutFilePaths.Add();
			info.myFileName = name;
			info.myFilePath = fullPath;
			info.myLastTimeModifiedLowbit = data.ftLastWriteTime.dwLowDateTime;
			info.myLastTimeModifiedHighbit = data.ftLastWriteTime.dwHighDateTime;

			RemoveFileExtention(name, info.myFileNameNoExtention);
		}
	} while (FindNextFile(filehandle, &data) != 0);

	if (GetLastError() != ERROR_NO_MORE_FILES)
		CE_ASSERT_ALWAYS("Something went wrong...");

	FindClose(filehandle);
	return true;
}

void CE_FileSystem::GetFileName(const CE_String& aFilePath, CE_String& aNameOut)
{
	int findIndex = aFilePath.RFind("/");
	aNameOut = aFilePath.SubStr(findIndex + 1, aFilePath.Lenght());
}

void CE_FileSystem::GetFileExtention(const CE_String& aFilePath, CE_String& aExtentionOut)
{
	int findIndex = aFilePath.RFind(".");
	aExtentionOut = aFilePath.SubStr(findIndex + 1, aFilePath.Lenght());
}

void CE_FileSystem::RemoveFileExtention(const CE_String& aFilePath, CE_String& aNameOut)
{
	int findIndex = aFilePath.RFind(".");
	aNameOut = aFilePath.SubStr(0, findIndex-1);
}

bool CE_FileSystem::GetFileInfo(const CE_String& aFilePath, FileInfo& aFileInfoOut)
{
	CE_ASSERT(aFilePath.Lenght() + 3 < MAX_PATH, "Filepath is too long");

	WIN32_FIND_DATA data;
	HANDLE filehandle = FindFirstFile(aFilePath.c_str(), &data);

	if (filehandle == INVALID_HANDLE_VALUE)
		return false;
	
	aFileInfoOut.myFileName = data.cFileName;
	aFileInfoOut.myFilePath = aFilePath;
	aFileInfoOut.myLastTimeModifiedLowbit = data.ftLastWriteTime.dwLowDateTime;
	aFileInfoOut.myLastTimeModifiedHighbit = data.ftLastWriteTime.dwHighDateTime;

	FindClose(filehandle);
	return true;
}

bool CE_FileSystem::UpdateFileInfo(CE_GrowingArray<FileInfo>& someFiles)
{
	bool somethingChanged = false;
	FileInfo newInfo;
	for (FileInfo& oldInfo : someFiles)
	{
		GetFileInfo(oldInfo.myFilePath, newInfo);

		FILETIME oldTime;
		oldTime.dwLowDateTime = oldInfo.myLastTimeModifiedLowbit;
		oldTime.dwHighDateTime = oldInfo.myLastTimeModifiedHighbit;

		FILETIME newTime;
		newTime.dwLowDateTime = newInfo.myLastTimeModifiedLowbit;
		newTime.dwHighDateTime = newInfo.myLastTimeModifiedHighbit;

		if (CompareFileTime(&oldTime, &newTime) != 0)
			somethingChanged = true;

		oldInfo.myLastTimeModifiedLowbit = newInfo.myLastTimeModifiedLowbit;
		oldInfo.myLastTimeModifiedHighbit = newInfo.myLastTimeModifiedHighbit;
	}

	return somethingChanged;
}
