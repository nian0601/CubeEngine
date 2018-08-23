#include "stdafx.h"
#include "CN_Script2In3OutNode.h"


CN_Script2In3OutNode::CN_Script2In3OutNode()
{
	AddPin<int>(0, true);
	AddPin<int>(1, true);

	AddPin<int>(2, false);
	AddPin<int>(3, false);
	AddPin<int>(4, false);

	myTempName = "2 In - 3 Out";
}

void CN_Script2In3OutNode::Execute(const CN_Pin& /*anExecutingPin*/)
{

}
