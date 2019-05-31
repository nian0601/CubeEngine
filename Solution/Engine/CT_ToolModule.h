#pragma once

#include "CPY_AABB.h"


// This should support having multiple selectable things,
// so that we can add multiple elements for a single Entity that
// we can select and do stuff with.
// Have something like this?
// int CreateToolEntity(CE_Entity aRealEntity)
// void AddInteractable(ToolEntity, Identifier/UserData, Transform*, Scale*, Min&, Max&)
// void RemoveInteractable(ToolEntity, Identifier)

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

	void AddToolEntity(CE_Entity anEntity, CE_Matrix44f* aTransform, CE_Vector3f* aScale, const CE_Vector3f& aMin, const CE_Vector3f& aMax);
	void RemoveToolEntity(CE_Entity anEntity);

private:
	void UpdateSelection();

	const CE_Camera& myCamera;
	const CE_Input& myInput;

	CT_Gizmo* myGizmo;
	CE_GrowingArray<CT_ToolEntity> myEntities;
	int mySelectedIndex;
};

