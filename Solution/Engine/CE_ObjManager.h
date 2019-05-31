#pragma once

#include "CE_AssetManager.h"

class CE_GPUContext;
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

	CE_Vector3f myMin;
	CE_Vector3f myMax;
};

class CE_ObjManager : public CE_AssetManager
{
public:
	static void Create(const CE_GPUContext& aGPUContext);
	static void Destroy();
	static CE_ObjManager* GetInstance() { return ourInstance; }

	const CE_ObjData* GetObjData(int aObjID) const;
	int GetObjID(const char* aObjName);

private:
	CE_ObjManager(const CE_GPUContext& aGPUContext);

	void LoadObj(const char* aObjName);

	CE_Map<CE_String, int> myNameToIDMap;
	CE_Map<int, CE_ObjData> myObjDataMap;
	int myNextFreeID;

	const CE_GPUContext& myGPUContext;

	static CE_ObjManager* ourInstance;
};
