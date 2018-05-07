#pragma once

#include "CE_TypeID.h"

class CE_World;
class CE_PathFinder;
class CE_Blackboard;

namespace CE_TypeRegistration
{
	void Register()
	{
		CE_TYPE_REGISTER(float);
		CE_TYPE_REGISTER(double);

		CE_TYPE_REGISTER(CE_Vector3f);
		CE_TYPE_REGISTER(CE_Vector4f);

		CE_TYPE_REGISTER(CE_World*);

		CE_TYPE_REGISTER(CE_PathFinder*);
		CE_TYPE_REGISTER(CE_Blackboard*);
	}
}