#include "stdafx.h"
#include "CN_Script3In3OutNode.h"


CN_Script3In3OutNode::CN_Script3In3OutNode()
{
	AddPin<int>(0, true);
	AddPin<int>(1, true);
	AddPin<int>(2, true);

	myPin1 = AddPin<int>(3, false);
	myPin2 = AddPin<int>(4, false);
	myPin3 = AddPin<int>(5, false);
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
