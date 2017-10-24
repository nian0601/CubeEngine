#pragma once

enum class eEntityTypes
{
	NONE,
	GROUND,
	PLAYER,
	RESOURCE_STONE,
	RESOURCE_WATER,
	GATHERER,
};

//////////////////////////////////////////////////////////////////////////
// Collision

namespace CollisionLayer
{
	enum eLayer
	{
		NONE = 0,
		CLICKABLE = 1 << 1,
		PLAYER = 1 << 2,
		PICKUP = 1 << 3,
	};

	inline eLayer FromString(const CE_String& aString)
	{
		if (aString == "CLICKABLE")
			return eLayer::CLICKABLE;
		else if (aString == "PLAYER")
			return eLayer::PLAYER;
		else if (aString == "PICKUP")
			return eLayer::PICKUP;

		return eLayer::NONE;
	}
};


//////////////////////////////////////////////////////////////////////////
// Resource

namespace ResourceType
{
	enum class eType
	{
		INVALID = -1,
		STONE,
		WATER
	};

	inline eType FromString(const CE_String& aString)
	{
		if (aString == "STONE")
			return eType::STONE;
		else if (aString == "WATER")
			return eType::WATER;

		CE_ASSERT_ALWAYS("Invalid Resource type");
		return eType::INVALID;
	}
};