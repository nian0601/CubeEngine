#include "stdafx.h"
#include "CN_ScriptDrawLineNode.h"
#include "CE_DebugDraw.h"
#include "CN_Pin.h"

CN_ScriptDrawLineNode::CN_ScriptDrawLineNode()
{
	AddPin<int>(0, true);
	AddPin<int>(1, true);
	AddPin<int>(2, true);
}

void CN_ScriptDrawLineNode::Execute(const CN_Pin& anExecutingPin)
{
	if (anExecutingPin.GetPinID() == 0)
	{
		CE_DRAW_LINE_COLOR(CE_Vector3f(0.f, 0.f, 0.f), CE_Vector3f(10.f, 10.f, 10.f), CE_Vector4f(0.4f, 1.f, 0.5f, 1.f));
	}
	else if (anExecutingPin.GetPinID() == 1)
	{
		CE_DRAW_LINE_COLOR(CE_Vector3f(0.f, 0.f, 0.f), CE_Vector3f(10.f, 5.f, 10.f), CE_Vector4f(1.0f, 0.4f, 0.5f, 1.f));
	}
	else if (anExecutingPin.GetPinID() == 2)
	{
		CE_DRAW_LINE_COLOR(CE_Vector3f(0.f, 0.f, 0.f), CE_Vector3f(10.f, 00.f, 10.f), CE_Vector4f(0.4f, 0.4f, 1.f, 1.f));
	}
}