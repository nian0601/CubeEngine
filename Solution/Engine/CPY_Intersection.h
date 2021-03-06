#pragma once

#include "CPY_AABB.h"
#include "CPY_Line3D.h"

namespace CPY_Intersection
{
	float LineVSAABB(const CPY_AABB& anAABB, const CPY_Line3D& aLine, CE_Vector3f* aOutIntersectionPoint = nullptr);
	bool AABBvsAABB(const CPY_AABB& anFirst, const CPY_AABB& anSecond);



	//Returns the squared distance to the collision point, -1 if no collision
	inline float LineVSAABB(const CPY_AABB& anAABB, const CPY_Line3D& aLine, CE_Vector3f* aOutIntersectionPoint)
	{
		CE_Vector3f rayOrg = aLine.myStart;
		CE_Vector3f rayDelta = aLine.myEnd - aLine.myStart;

		bool inside = true;

		float xt;
		float xn;

		if (rayOrg.x < anAABB.myMinPos.x)
		{
			xt = anAABB.myMinPos.x - rayOrg.x;
			if (xt > rayDelta.x)
			{
				return -1.f;
			}
			xt /= rayDelta.x;
			inside = false;
			xn = -1.f;
		}
		else if (rayOrg.x > anAABB.myMaxPos.x)
		{
			xt = anAABB.myMaxPos.x - rayOrg.x;
			if (xt < rayDelta.x)
			{
				return -1.f;
			}
			xt /= rayDelta.x;
			inside = false;
			xn = 1.f;
		}
		else
		{
			xt = -1.f;
		}

		float yt;
		float yn;

		if (rayOrg.y < anAABB.myMinPos.y)
		{
			yt = anAABB.myMinPos.y - rayOrg.y;
			if (yt > rayDelta.y)
			{
				return -1.f;
			}
			yt /= rayDelta.y;
			inside = false;
			yn = -1.f;
		}
		else if (rayOrg.y > anAABB.myMaxPos.y)
		{
			yt = anAABB.myMaxPos.y - rayOrg.y;
			if (yt < rayDelta.y)
			{
				return -1.f;
			}
			yt /= rayDelta.y;
			inside = false;
			yn = 1.f;
		}
		else
		{
			yt = -1.f;
		}

		float zt;
		float zn;

		if (rayOrg.z < anAABB.myMinPos.z)
		{
			zt = anAABB.myMinPos.z - rayOrg.z;
			if (zt > rayDelta.z)
			{
				return -1.f;
			}
			zt /= rayDelta.z;
			inside = false;
			zn = -1.f;
		}
		else if (rayOrg.z > anAABB.myMaxPos.z)
		{
			zt = anAABB.myMaxPos.z - rayOrg.z;
			if (zt < rayDelta.z)
			{
				return -1.f;
			}
			zt /= rayDelta.z;
			inside = false;
			zn = 1.f;
		}
		else
		{
			zt = -1.f;
		}

		if (inside == true)
		{
			if(aOutIntersectionPoint)
				*aOutIntersectionPoint = aLine.myStart;

			return 0.f;
		}

		int which = 0;

		float t = xt;

		if (yt > t)
		{
			which = 1;
			t = yt;
		}

		if (zt > t)
		{
			which = 2;
			t = zt;
		}

		switch (which)
		{
		case 0:
		{
			float y = rayOrg.y + rayDelta.y * t;
			if (y < anAABB.myMinPos.y || y > anAABB.myMaxPos.y)
			{
				return -1.f;
			}
			float z = rayOrg.z + rayDelta.z * t;
			if (z < anAABB.myMinPos.z || z > anAABB.myMaxPos.z)
			{
				return -1.f;
			}
			break;
		}
		case 1:
		{
			float x = rayOrg.x + rayDelta.x * t;
			if (x < anAABB.myMinPos.x || x > anAABB.myMaxPos.x)
			{
				return -1.f;
			}
			float z = rayOrg.z + rayDelta.z * t;
			if (z < anAABB.myMinPos.z || z > anAABB.myMaxPos.z)
			{
				return -1.f;
			}
			break;
		}
		case 2:
		{
			float x = rayOrg.x + rayDelta.x * t;
			if (x < anAABB.myMinPos.x || x > anAABB.myMaxPos.x)
			{
				return -1.f;
			}
			float y = rayOrg.y + rayDelta.y * t;
			if (y < anAABB.myMinPos.y || y > anAABB.myMaxPos.y)
			{
				return -1.f;
			}
			break;
		}
		}

		CE_Vector3f intersectPoint = aLine.myStart + rayDelta * t;
		if (aOutIntersectionPoint)
			*aOutIntersectionPoint = intersectPoint;

		return CE_Length2(intersectPoint - aLine.myStart);
	}

	inline bool AABBvsAABB(const CPY_AABB& anFirst, const CPY_AABB& anSecond)
	{
		if (anFirst.myMaxPos.x < anSecond.myMinPos.x) return false;
		if (anFirst.myMaxPos.y < anSecond.myMinPos.y) return false;
		if (anFirst.myMaxPos.z < anSecond.myMinPos.z) return false;
		if (anFirst.myMinPos.x > anSecond.myMaxPos.x) return false;
		if (anFirst.myMinPos.y > anSecond.myMaxPos.y) return false;
		if (anFirst.myMinPos.z > anSecond.myMaxPos.z) return false;

		return true;
	}
};

