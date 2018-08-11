#include "stdafx.h"
#include "CN_Script2In3OutNode.h"


CN_Script2In3OutNode::CN_Script2In3OutNode()
{
	AddPin(0, 0, true);
	AddPin(0, 1, true);

	AddPin(0, 2, false);
	AddPin(0, 3, false);
	AddPin(0, 4, false);

	myTempName = "2 In - 3 Out";
}

void CN_Script2In3OutNode::Execute(const CN_Pin& /*anExecutingPin*/)
{

}
