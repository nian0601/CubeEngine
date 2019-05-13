#pragma once

#include "CE_TypeReflection.h"

class CE_World;
class CE_PathFinder;
class CE_Blackboard;

class CUI_Pin;

namespace CE_TypeRegistration
{
	void Register()
	{
		CE_TYPE_REGISTER(int, CE_Vector4f(0.2f, 0.2f, 0.5f, 1.f));
		CE_TYPE_REGISTER(float, CE_Vector4f(0.2f, 0.5f, 0.2f, 1.f));
		CE_TYPE_REGISTER(double, CE_Vector4f(0.2f, 0.2f, 0.2f, 1.f));

		CE_TYPE_REGISTER(CE_Vector2f, CE_Vector4f(0.7f, 0.5f, 0.2f, 1.f));
		CE_TYPE_REGISTER(CE_Vector3f, CE_Vector4f(0.5f, 0.5f, 0.2f, 1.f));
		CE_TYPE_REGISTER(CE_Vector4f, CE_Vector4f(0.5f, 0.5f, 0.7f, 1.f));

		CE_TYPE_REGISTER(CE_World*, CE_Vector4f(0.3f, 0.3f, 0.f, 1.f));

		CE_TYPE_REGISTER(CE_PathFinder*, CE_Vector4f(0.3f, 0.f, 0.5f, 1.f));
		CE_TYPE_REGISTER(CE_Blackboard*, CE_Vector4f(0.f, 0.3f, 0.5f, 1.f));

		CE_TYPE_REGISTER_NO_COLOR(CUI_Pin*);
	}
}