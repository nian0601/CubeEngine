#pragma once

#include "CN_Node.h"

class CN_Script3In3OutNode : public CN_Node
{
	CN_DEFINE_NODE(CN_Script3In3OutNode);

public:
	CN_Script3In3OutNode();

	void Execute(const CN_Pin& anExecutingPin) override;

private:
	CN_Pin* myPin1;
	CN_Pin* myPin2;
	CN_Pin* myPin3;
};

