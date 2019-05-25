#include "stdafx.h"
#include "CE_ObjManager.h"
#include "CE_FileSystem.h"
#include "CE_ObjLoader.h"
#include "CE_RenderObject.h"
#include "CE_MaterialManager.h"

CE_ObjManager::CE_ObjManager(const char* aObjFolder, const CE_GPUContext& aGPUContext, CE_MaterialManager& aMaterialManager)
	: CE_AssetManager(aObjFolder, true)
	, myNextFreeID(0)
	, myGPUContext(aGPUContext)
	, myMaterialManager(aMaterialManager)
{
}

const CE_ObjData* CE_ObjManager::GetObjData(int aObjID) const
{
	if (const CE_ObjData* data = myObjDataMap.GetIfExists(aObjID))
		return data;

	CE_ASSERT(false, "Obj with ID %i isnt loaded, how did you get this ID??", aObjID);
	return nullptr;
}

int CE_ObjManager::GetObjID(const char* aObjName)
{
	if (!myNameToIDMap.KeyExists(aObjName))
		LoadObj(aObjName);

	return myNameToIDMap[aObjName];
}

void CE_ObjManager::LoadObj(const char* aObjName)
{
	const CE_String* fullFilePath = myFullFilePaths.GetIfExists(aObjName);
	CE_ASSERT(fullFilePath, "Couldnt find full filepath for %s??", aObjName);

	int objID = myNextFreeID++;
	myNameToIDMap[aObjName] = objID;

	CE_ObjData& objData = myObjDataMap[objID];

	CE_ObjLoader loader;
	loader.Load(fullFilePath->c_str());

	for (CE_ObjLoader::Model& model : loader.myModels)
	{
		CE_ObjMesh& mesh = objData.myMeshes.Add();
		mesh.myRenderObject = new CE_RenderObject();
		mesh.myRenderObject->Init<CE_PosNormColor_Vert>(myGPUContext, (void*)model.myVertices.GetArrayAsPointer(), model.myVertices.Size(), (void*)model.myIndices.GetArrayAsPointer(), model.myIndices.Size());
		mesh.myMaterial = myMaterialManager.GetMaterial(model.myMaterial.c_str());
	}
}

