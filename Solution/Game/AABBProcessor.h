#pragma once

#include <CE_BaseProcessor.h>

class CPY_PhysicsWorld;
class AABBProcessor : public CE_BaseProcessor
{
public:
	AABBProcessor(CE_World& aWorld, CPY_PhysicsWorld& aPhysicsWorld);

	void Update(float aDelta) override;

	void EntityAdded(CE_Entity anEntity) override;

private:
	void UpdateAABBs(const CE_GrowingArray<CE_Entity>& someEntities);
	void UpdateCollisions(const CE_GrowingArray<CE_Entity>& someEntities);

	CPY_PhysicsWorld& myPhysicsWorld;
};

