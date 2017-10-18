#pragma once

enum class eEntityTypes
{
	NONE,
	GROUND,
	PLAYER,
	PICK_UP,
	MOVER,
};

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