#include "stdafx.h"
#include "CE_ObjManager.h"
#include "CE_FileSystem.h"
#include "CE_ObjLoader.h"
#include "CE_RenderObject.h"
#include "CE_MaterialManager.h"

CE_ObjManager* CE_ObjManager::ourInstance = nullptr;
void CE_ObjManager::Create(const CE_GPUContext& aGPUContext)
{
	CE_ASSERT(ourInstance == nullptr, "Tried to create ObjManager twice!");
	ourInstance = new CE_ObjManager(aGPUContext);
}

void CE_ObjManager::Destroy()
{
	CE_SAFE_DELETE(ourInstance);
}


CE_ObjManager::CE_ObjManager(const CE_GPUContext& aGPUContext)
	: CE_AssetManager("Data/Models", true)
	, myNextFreeID(0)
	, myGPUContext(aGPUContext)
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
		mesh.myMaterial = CE_MaterialManager::GetInstance()->GetMaterial(model.myMaterial.c_str());

		if (model.myMin.x < objData.myMin.x) objData.myMin.x = model.myMin.x;
		if (model.myMin.y < objData.myMin.y) objData.myMin.y = model.myMin.y;
		if (model.myMin.z < objData.myMin.z) objData.myMin.z = model.myMin.z;

		if (model.myMin.x > objData.myMax.x) objData.myMax.x = model.myMax.x;
		if (model.myMin.y > objData.myMax.y) objData.myMax.y = model.myMax.y;
		if (model.myMin.z > objData.myMax.z) objData.myMax.z = model.myMax.z;
	}
}

