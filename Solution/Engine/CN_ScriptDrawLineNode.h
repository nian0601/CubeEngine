#pragma once

#include "CN_Node.h"

class CN_ScriptDrawLineNode : public CN_Node
{
public:
	CN_ScriptDrawLineNode();

	void Execute(const CN_Pin& anExecutingPin) override;
};

