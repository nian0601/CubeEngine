#pragma once

#include "CPY_AABB.h"

class CE_Camera;
class CE_Input;
class CE_RendererProxy;

class CT_Gizmo
{
public:
	CT_Gizmo(const CE_Camera& aCamera);

	bool Update(const CE_Input& anInput, float aDelta);
	void Render(CE_RendererProxy& aRenderProxy);

	void SetTransformAndScale(CE_Matrix44f* aTransform, CE_Vector3f* aScale);
private:
	void UpdateMouseDrag(const CE_Input& anInput, float aDelta);
	void UpdateTranslation(const CE_Input& anInput, float aMouseMovement, bool aCanDoSnapMove, float aDelta);
	void UpdateScale(const CE_Input& anInput, float aMouseMovement, bool aCanDoSnapMove, float aDelta);

	void UpdateAABB();
	int GetIndexUnderMouse(const CE_Input& anInput) const;

	const CE_Camera& myCamera;
	const float myBarSize;
	const float myBarHalfSize;
	CE_Matrix44f* myTransform;
	CE_Vector3f* myScale;
	CE_Vector4f myColors[3];
	CPY_AABB myAABBs[3];

	int myHoveredIndex;
	int mySelectedIndex;
	float myAccumulatedMouseMovementThreshold;
};

