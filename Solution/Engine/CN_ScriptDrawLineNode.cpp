#include "stdafx.h"
#include "CN_ScriptDrawLineNode.h"
#include "CE_DebugDraw.h"
#include "CN_Pin.h"

CN_ScriptDrawLineNode::CN_ScriptDrawLineNode()
{
	myColorInput = AddPin<CE_Vector4f>(1, true, "Color");
	myStartInput = AddPin<CE_Vector2f>(2, true, "Start");
	myEndInput = AddPin<CE_Vector2f>(3, true, "End");

	AddPin<CN_ExecutionImpulse>(4, true, "Draw");
}

void CN_ScriptDrawLineNode::Execute(const CN_Pin& /*anExecutingPin*/)
{
	CE_Vector4f color = myColorInput->Read<CE_Vector4f>();
	CE_Vector2f pos1 = myStartInput->Read<CE_Vector2f>();
	CE_Vector2f pos2 = myEndInput->Read<CE_Vector2f>();

	CE_DRAW_LINE_COLOR(CE_Vector3f(pos1.x, 0.f, pos1.y), CE_Vector3f(pos2.x, 0.f, pos2.y), color);
}