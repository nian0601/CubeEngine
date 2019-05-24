#include "stdafx.h"
#include "CE_ObjManager.h"
#include "CE_FileSystem.h"
#include "CE_ObjLoader.h"
#include "CE_RenderObject.h"
#include "CE_MaterialManager.h"

CE_ObjManager::CE_ObjManager(const char* aObjFolder, const CE_GPUContext& aGPUContext, const CE_MaterialManager& aMaterialManager)
	: myNextFreeID(0)
	, myMaterialManager(aMaterialManager)
{
	// We do this same in all the different asset-managers,
	// just create a base AssetManager-class that handles searching for all the files
	// and just calls a virtual LoadAsset-function?

	CE_GrowingArray<CE_FileSystem::FileInfo> files;
	CE_FileSystem::GetAllFilesFromDirectory(aObjFolder, files);

	for (const CE_FileSystem::FileInfo& fileInfo : files)
		LoadObj(fileInfo.myFilePath.c_str(), aGPUContext);
}

const CE_ObjData* CE_ObjManager::GetObjData(int aObjID) const
{
	if (const CE_ObjData* data = myObjDataMap.GetIfExists(aObjID))
		return data;

	CE_ASSERT(false, "Obj with ID %i isnt loaded, how did you get this ID??", aObjID);
	return nullptr;
}

int CE_ObjManager::GetObjID(const char* aObjName) const
{
	if (const int* id = myNameToIDMap.GetIfExists(aObjName))
		return *id;

	CE_ASSERT(false, "Obj %s wasnt loaded, make sure its placed in the Models-folder!", aObjName);
	return -1;
}

void CE_ObjManager::LoadObj(const char* aFilePath, const CE_GPUContext& aGPUContext)
{
	CE_String fileName;
	CE_FileSystem::GetFileName(aFilePath, fileName);

	CE_ASSERT(myNameToIDMap.GetIfExists(fileName) == nullptr, "Obj %s was already loaded!", fileName);

	int objID = myNextFreeID++;
	myNameToIDMap[fileName] = objID;

	CE_ObjData& objData = myObjDataMap[objID];

	CE_ObjLoader loader;
	loader.Load(aFilePath);

	for (CE_ObjLoader::Model& model : loader.myModels)
	{
		CE_ObjMesh& mesh = objData.myMeshes.Add();
		mesh.myRenderObject = new CE_RenderObject();
		mesh.myRenderObject->Init<CE_PosNormColor_Vert>(aGPUContext, (void*)model.myVertices.GetArrayAsPointer(), model.myVertices.Size(), (void*)model.myIndices.GetArrayAsPointer(), model.myIndices.Size());
		mesh.myMaterial = myMaterialManager.GetMaterial(model.myMaterial.c_str());
	}
}

