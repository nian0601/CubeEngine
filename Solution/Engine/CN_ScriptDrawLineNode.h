#pragma once

#include "CN_Node.h"

class CN_ScriptDrawLineNode : public CN_Node
{
	CN_DEFINE_NODE(CN_ScriptDrawLineNode);

public:
	CN_ScriptDrawLineNode();

	void Execute(const CN_Pin& anExecutingPin) override;

private:
	CN_Pin* myStartInput;
	CN_Pin* myEndInput;
	CN_Pin* myColorInput;
};

