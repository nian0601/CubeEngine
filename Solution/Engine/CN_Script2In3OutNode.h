#pragma once

#include "CN_Node.h"

class CN_Script2In3OutNode : public CN_Node
{
	CN_DEFINE_NODE(CN_Script2In3OutNode);

public:
	CN_Script2In3OutNode();

	void Execute(const CN_Pin& anExecutingPin) override;
};

