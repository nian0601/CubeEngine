#include "stdafx.h"

#include "CE_AssetManager.h"

CE_AssetManager::CE_AssetManager(const char* aAssetFolder, bool aIncludeExtentionInKey)
{
	CE_FileSystem::GetAllFilesFromDirectory(aAssetFolder, myFiles);

	for (const CE_FileSystem::FileInfo& fileInfo : myFiles)
	{
		CE_ASSERT(myFullFilePaths.KeyExists(fileInfo.myFileName) == false, "Find two files with the same name [%s]", fileInfo.myFileName.c_str());
		if(aIncludeExtentionInKey)
			myFullFilePaths[fileInfo.myFileName] = fileInfo.myFilePath;
		else 
			myFullFilePaths[fileInfo.myFileNameNoExtention] = fileInfo.myFilePath;
	}
}

