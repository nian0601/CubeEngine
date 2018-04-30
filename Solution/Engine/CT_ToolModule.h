#pragma once

#include "CPY_AABB.h"

struct CT_ToolEntity
{
	CE_Entity myRealEntity;
	CE_Matrix44f* myTransform;
	CE_Vector3f* myScale;

	CPY_AABB myAABB;
};

class CE_Camera;
class CE_Input;
class CE_RendererProxy;

class CT_Gizmo;

class CT_ToolModule
{
public:
	CT_ToolModule(const CE_Camera& aCamera, const CE_Input& anInput);
	~CT_ToolModule();

	void Update(float aDelta);
	void Render(CE_RendererProxy& aRenderProxy);

	void AddToolEntity(CE_Entity anEntity, CE_Matrix44f* aTransform, CE_Vector3f* aScale);
	void RemoveToolEntity(CE_Entity anEntity);

private:
	void UpdateSelection();
	void UpdateAABB(CT_ToolEntity& anEntity);

	const CE_Camera& myCamera;
	const CE_Input& myInput;

	CT_Gizmo* myGizmo;
	CE_GrowingArray<CT_ToolEntity> myEntities;
	int mySelectedIndex;
};

