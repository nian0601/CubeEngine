#include "stdafx.h"

#include "CN_ScriptInitNode.h"
#include "CN_Pin.h"

CN_ScriptInitNode::CN_ScriptInitNode()
{
	myPin = AddPin<int>(0, false, "Init");
}

void CN_ScriptInitNode::Execute(const CN_Pin& /*anExecutingPin*/)
{
	myPin->Execute();
}

void CN_ScriptInitNode::Execute()
{
	myPin->Execute();
}
