#include "stdafx.h"
#include "CN_Node.h"
#include "CN_Pin.h"

CN_Node::CN_Node()
{
}

CN_Node::~CN_Node()
{
	myAllPins.DeleteAll();
}

CN_Pin* CN_Node::GetPin(u32 aPinIndex)
{
	for (CN_Pin* pin : myAllPins)
	{
		if (pin->GetPinID() == aPinIndex)
			return pin;
	}

	return nullptr;
}
