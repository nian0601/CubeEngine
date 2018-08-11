#include "stdafx.h"
#include "CN_Pin.h"
#include "CN_Node.h"

CN_Pin::CN_Pin(u32 aDataType, u32 aPinID, bool aIsInput, CN_Node* aNode)
	: myDataType(aDataType)
	, myPinID(aPinID)
	, myIsInput(aIsInput)
	, myNode(aNode)
{
}

void CN_Pin::Execute()
{
	CE_ASSERT(!myIsInput, "Tried to execute an input-pin. That doesnt work!");

	for (CN_Pin* output : myConnectedPins)
	{
		CN_Node* node = output->myNode;
		node->Execute(*output);
	}
}
