#pragma once

#include <CE_BaseProcessor.h>

class CE_Camera;
class SelectionProcessor : public CE_BaseProcessor
{
public:
	SelectionProcessor(CE_World& aWorld, const CE_Camera& aCamera);

	void Update(float aDelta) override;

private:
	CE_Entity FindEntityUnderMouse(const CE_Vector2f& aMousePosition, CE_Vector3f& aIntersectionPointOut);
	CE_Vector3f Unproject(const CE_Vector2f& aPosition, float aDepth) const;

	const CE_Camera& myCamera;
};

