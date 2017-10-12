#pragma once

#include <CE_BaseComponent.h>
#include <CPY_AABB.h>

struct AABBComponent : public CE_BaseComponent
{
	CPY_AABB myAABB;
};
