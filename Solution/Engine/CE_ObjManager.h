#pragma once

#include "CE_AssetManager.h"

class CE_GPUContext;
class CE_MaterialManager;
class CE_RenderObject;
struct CE_Material;

struct CE_ObjMesh
{
	CE_RenderObject* myRenderObject;
	const CE_Material* myMaterial;
};

struct CE_ObjData
{
	CE_GrowingArray<CE_ObjMesh> myMeshes;
};

class CE_ObjManager : public CE_AssetManager
{
public:
	CE_ObjManager(const char* aObjFolder, const CE_GPUContext& aGPUContext, CE_MaterialManager& aMaterialManager);

	const CE_ObjData* GetObjData(int aObjID) const;
	int GetObjID(const char* aObjName);

private:
	void LoadObj(const char* aObjName);

	CE_Map<CE_String, int> myNameToIDMap;
	CE_Map<int, CE_ObjData> myObjDataMap;
	int myNextFreeID;

	const CE_GPUContext& myGPUContext;
	CE_MaterialManager& myMaterialManager;
};
