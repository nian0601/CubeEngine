#include "stdafx.h"
#include "CE_MaterialManager.h"
#include "CE_FileSystem.h"
#include "CE_FileParser.h"

CE_MaterialManager::CE_MaterialManager(const char* aMaterialsPath)
{
	CE_GrowingArray<CE_FileSystem::FileInfo> files;
	CE_FileSystem::GetAllFilesFromDirectory(aMaterialsPath, files);

	for (const CE_FileSystem::FileInfo& fileInfo : files)
	{
		LoadMaterial(fileInfo.myFilePath.c_str());
	}
}

const CE_Material* CE_MaterialManager::GetMaterial(const char* aMaterialName) const
{
	if (const CE_Material* material = myMaterials.GetIfExists(aMaterialName))
		return material;

	CE_ASSERT("Material %s was not loaded, make sure its present in the Materials Folder!", aMaterialName);
	return nullptr;
}

void CE_MaterialManager::LoadMaterial(const char* aMaterialPath)
{
	CE_FileParser file(aMaterialPath);

	CE_String line;
	CE_GrowingArray<CE_String> words;

	CE_Material* currMaterial = nullptr;
	while (file.ReadLine(line))
	{
		file.TrimBeginAndEnd(line);
		file.SplitLine(line, words);

		if (words[0].Empty() || words[0] == "#")
			continue;

		if (words[0] == "newmtl")
		{
			currMaterial = &myMaterials[words[1]];
			currMaterial->myMetalness = 0.f;
			currMaterial->myRoughness = 1.f;
		}
		else 
		{
			CE_ASSERT(currMaterial != nullptr, "Dont have a material??");

			CE_Vector4f* vector = nullptr;
			if (words[0] == "Ka")
				vector = &currMaterial->myAlbedo;
			else if (words[0] == "Kd")
				vector = &currMaterial->myDiffuse;
			else if (words[0] == "Ks")
				vector = &currMaterial->mySpecular;
			else
				CE_ASSERT(false, "Unhandled Material property");

			vector->x = file.GetFloat(words[1]);
			vector->y = file.GetFloat(words[2]);
			vector->z = file.GetFloat(words[3]);
			vector->w = 1.f;
		}
	}
}
