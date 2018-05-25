#include "stdafx.h"
#include "CN_ScriptDrawLineNode.h"


CN_ScriptDrawLineNode::CN_ScriptDrawLineNode()
{
	AddPin(0, 0, true);
	AddPin(1, 1, true);
	AddPin(1, 2, true);

	myTempName = "Line Node";
}