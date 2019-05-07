#include "stdafx.h"
#include "CN_ScriptDrawLineNode.h"
#include "CE_DebugDraw.h"
#include "CN_Pin.h"

CN_ScriptDrawLineNode::CN_ScriptDrawLineNode()
{
	AddPin<int>(0, true, "Green");
	AddPin<int>(1, true, "Red");
	AddPin<int>(2, true, "Blue");

	myInputOne = AddPin<CE_Vector2f>(3, true, "Start");
	myInputTwo = AddPin<CE_Vector2f>(4, true, "End");
}

void CN_ScriptDrawLineNode::Execute(const CN_Pin& anExecutingPin)
{
	CE_Vector2f pos1 = myInputOne->Read<CE_Vector2f>();
	CE_Vector2f pos2 = myInputTwo->Read<CE_Vector2f>();

	CE_Vector4f color(0.f, 0.f, 0.f, 1.f);
	if (anExecutingPin.GetPinID() == 0)
		color = CE_Vector4f(0.4f, 1.f, 0.5f, 1.f);
	else if (anExecutingPin.GetPinID() == 1)
		color = CE_Vector4f(1.0f, 0.4f, 0.5f, 1.f);
	else if (anExecutingPin.GetPinID() == 2)
		color = CE_Vector4f(0.4f, 0.4f, 1.f, 1.f);

	CE_DRAW_LINE_COLOR(CE_Vector3f(pos1.x, 0.f, pos1.y), CE_Vector3f(pos2.x, 0.f, pos2.y), color);
}