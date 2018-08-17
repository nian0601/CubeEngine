#pragma once

#include "CN_Node.h"

class CN_ScriptInitNode : public CN_Node
{
	CN_DEFINE_NODE(CN_ScriptInitNode);

public:
	CN_ScriptInitNode();

	void Execute();
	void Execute(const CN_Pin& anExecutingPin) override;

private:
	CN_Pin* myPin;
};

