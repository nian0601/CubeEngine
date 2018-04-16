#pragma once

enum class eEntityTypes
{
	NONE,
	GROUND,
	PLAYER,
	RESOURCE_STONE,
	RESOURCE_WATER,
	GATHERER,
	STOCKPILE,
	SPHERE,
	TREE,
	POINT_LIGHT,
	PROJECTILE,
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

//////////////////////////////////////////////////////////////////////////
// Model Type

namespace ModelType
{
	enum class eType
	{
		INVALID = -1,
		CUBE,
		SPHERE,
		POINT_LIGHT,
	};

	inline eType FromString(const CE_String& aString)
	{
		if (aString == "CUBE")
			return eType::CUBE;
		else if (aString == "SPHERE")
			return eType::SPHERE;
		else if (aString == "POINTLIGHT")
			return eType::POINT_LIGHT;

		CE_ASSERT_ALWAYS("Invalid Model type");
		return eType::INVALID;
	}
};