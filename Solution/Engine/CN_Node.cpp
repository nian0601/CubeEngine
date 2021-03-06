#include "stdafx.h"
#include "CN_Node.h"
#include "CN_Pin.h"

CN_Node::CN_Node()
{
}

CN_Node::~CN_Node()
{
	for (CN_Pin* pin : myAllPins)
	{
		CE_GrowingArray<CN_Pin*>& connections = pin->GetConnectedPins();
		for (CN_Pin* connection : connections)
			connection->GetConnectedPins().RemoveCyclic(pin);
	}

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
