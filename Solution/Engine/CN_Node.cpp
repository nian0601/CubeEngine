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
	for (CN_Pin* input : myInputPins)
	{
		if (input->GetPinID() == aPinIndex)
			return input;
	}

	for (CN_Pin* output : myOutputPins)
	{
		if (output->GetPinID() == aPinIndex)
			return output;
	}

	return nullptr;
}

void CN_Node::AddPin(u32 aDataType, u32 aPinID, bool aIsInput)
{
	CN_Pin* pin = new CN_Pin(aDataType, aPinID, aIsInput, this);
	if (pin->GetIsInput())
		myInputPins.Add(pin);
	else
		myOutputPins.Add(pin);

	myAllPins.Add(pin);
}
