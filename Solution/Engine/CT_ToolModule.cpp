#include "stdafx.h"

#include "CE_Camera.h"
#include "CE_Input.h"

#include "CT_ToolModule.h"
#include "CT_Gizmo.h"

#include "CPY_Intersection.h"

CT_ToolModule::CT_ToolModule(const CE_Camera& aCamera, const CE_Input& anInput)
	: myCamera(aCamera)
	, myInput(anInput)
{
	myGizmo = new CT_Gizmo(aCamera);
}

CT_ToolModule::~CT_ToolModule()
{
	CE_SAFE_DELETE(myGizmo);
}

void CT_ToolModule::Update(float aDelta)
{
	for (CT_ToolEntity& entity : myEntities)
		UpdateAABB(entity);

	if (!myGizmo->Update(myInput, aDelta))
	{
		UpdateSelection();
	}
}

void CT_ToolModule::Render(CE_RendererProxy& aRenderProxy)
{
	myGizmo->Render(aRenderProxy);
}

void CT_ToolModule::AddToolEntity(CE_Entity anEntity, CE_Matrix44f* aTransform, CE_Vector3f* aScale)
{
	CT_ToolEntity& entity = myEntities.Add();
	entity.myRealEntity = anEntity;
	entity.myTransform = aTransform;
	entity.myScale = aScale;
	UpdateAABB(entity);
}

void CT_ToolModule::RemoveToolEntity(CE_Entity anEntity)
{
	for (int i = 0; i < myEntities.Size(); ++i)
	{
		if (myEntities[i].myRealEntity == anEntity)
		{
			if (mySelectedIndex == i)
			{
				mySelectedIndex = -1;
				myGizmo->SetTransformAndScale(nullptr, nullptr);
			}

			myEntities.RemoveCyclicAtIndex(i);
			return;
		}
	}
}

void CT_ToolModule::UpdateSelection()
{
	if (!myInput.MouseDown(0))
		return;

	CE_Vector2f mousePosition = myInput.GetMousePosition();

	CE_Vector3f lineStart = myCamera.UnprojectPosition(mousePosition, 0.f);
	CE_Vector3f lineEnd = myCamera.UnprojectPosition(mousePosition, 1.f);
	CPY_Line3D line(lineStart, lineEnd);

	mySelectedIndex = -1;
	myGizmo->SetTransformAndScale(nullptr, nullptr);

	float bestDistance = FLT_MAX;
	for(int i = 0; i < myEntities.Size(); ++i)
	{
		const CT_ToolEntity& toolEntity = myEntities[i];
		float distance = CPY_Intersection::LineVSAABB(toolEntity.myAABB, line);
		if (distance >= 0.f && distance < bestDistance)
		{
			bestDistance = distance;

			mySelectedIndex = i;
			myGizmo->SetTransformAndScale(toolEntity.myTransform, toolEntity.myScale);
		}
	}
}

void CT_ToolModule::UpdateAABB(CT_ToolEntity& anEntity)
{
	anEntity.myAABB.myExtents = *anEntity.myScale;
	anEntity.myAABB.Move(anEntity.myTransform->GetPos());
}
