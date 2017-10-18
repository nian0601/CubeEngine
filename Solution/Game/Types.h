#pragma once

enum class eEntityTypes
{
	NONE,
	GROUND,
	PLAYER,
	PICK_UP,
	MOVER,
	RESOURCE_STONE,
	RESOURCE_WATER,
};

//////////////////////////////////////////////////////////////////////////
// Collision

enum eCollisionLayer
{
	NONE = 0,
	CLICKABLE = 1 << 1,
	PLAYER = 1 << 2,
	PICKUP = 1 << 3,
};

inline eCollisionLayer ConvertStringToCollisionLayer(const CE_String& aString)
{
	if (aString == "CLICKABLE")
		return eCollisionLayer::CLICKABLE;
	else if (aString == "PLAYER")
		return eCollisionLayer::PLAYER;
	else if (aString == "PICKUP")
		return eCollisionLayer::PICKUP;

	return eCollisionLayer::NONE;
}

//////////////////////////////////////////////////////////////////////////
// Resource

enum class eResourceType
{
	INVALID = -1,
	STONE,
	WATER
};

inline eResourceType ConvertStringToResourceType(const CE_String& aString)
{
	if (aString == "STONE")
		return eResourceType::STONE;
	else if (aString == "WATER")
		return eResourceType::WATER;

	CE_ASSERT_ALWAYS("Invalid Resource type");
	return eResourceType::INVALID;
}