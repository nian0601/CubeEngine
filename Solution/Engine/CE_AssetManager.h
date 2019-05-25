#pragma once
#include "CE_FileSystem.h"

class CE_AssetManager
{
public:
	virtual ~CE_AssetManager() {};

protected:
	CE_AssetManager(const char* aAssetFolder, bool aIncludeExtentionInKey);

	CE_Map<CE_String, CE_String> myFullFilePaths;
	CE_GrowingArray<CE_FileSystem::FileInfo> myFiles;
};