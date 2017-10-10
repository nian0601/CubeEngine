#pragma once

#include <CE_BaseComponent.h>
#include <CPY_AABB.h>

struct CollisionComponent : public CE_BaseComponent
{
	CPY_AABB myAABB;

	float myRadius;
};