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

CN_Pin* CN_Node::AddPin(u32 aDataType, u32 aPinID, bool aIsInput)
{
	CN_Pin* pin = new CN_Pin(aDataType, aPinID, aIsInput, this);
	myAllPins.Add(pin);

	return pin;
}
