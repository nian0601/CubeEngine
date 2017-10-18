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
	GROUNND = 1 << 1,
};