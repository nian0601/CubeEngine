#pragma once

class CPY_CollisionEntity;

struct CPY_CollisionEntry
{
	CPY_CollisionEntry() {}
	CPY_CollisionEntry(const CPY_CollisionEntity* aFirst, const CPY_CollisionEntity* aSecond)
		: myFirstEntity(aFirst)
		, mySecondEntity(aSecond)
	{}

	const CPY_CollisionEntity* myFirstEntity;
	const CPY_CollisionEntity* mySecondEntity;
};

class CPY_PhysicsWorld
{
public:
	CPY_PhysicsWorld();
	~CPY_PhysicsWorld();

	void Update();

	CPY_CollisionEntity* CreateCollisionEntity(CE_Entity anEntity);

	const CE_GrowingArray<CPY_CollisionEntry>& GetCollisionEntries() const { return myCollisionEntries; }
	const CPY_CollisionEntity* RayCast(const CE_Vector3f& aRayStart, const CE_Vector3f& aRayEnd, int someCollisionFlags, CE_Vector3f& aIntersectionPointOut) const;

private:
	CE_GrowingArray<CPY_CollisionEntity*> myCollisionEntities;
	CE_GrowingArray<CPY_CollisionEntry> myCollisionEntries;
};

