#include "stdafx.h"
#include "CN_Script3In3OutNode.h"


CN_Script3In3OutNode::CN_Script3In3OutNode()
{
	AddPin<int>(0, true, "In 1");
	AddPin<int>(1, true, "In 2");
	AddPin<int>(2, true, "In 3");

	myPin1 = AddPin<int>(3, false, "Out 1");
	myPin2 = AddPin<int>(4, false, "Out 2");
	myPin3 = AddPin<int>(5, false, "Out 3");
}

void CN_Script3In3OutNode::Execute(const CN_Pin& anExecutingPin)
{
	if (anExecutingPin.GetPinID() == 0)
		myPin1->Execute();
	else if (anExecutingPin.GetPinID() == 1)
		myPin2->Execute();
	else if (anExecutingPin.GetPinID() == 2)
		myPin3->Execute();
}
