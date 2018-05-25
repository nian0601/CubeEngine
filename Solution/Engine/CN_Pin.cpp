#include "stdafx.h"
#include "CN_Pin.h"

CN_Pin::CN_Pin(u32 aDataType, u32 aPinID, bool aIsInput, CN_Node* aNode)
	: myDataType(aDataType)
	, myPinID(aPinID)
	, myIsInput(aIsInput)
	, myNode(aNode)
{
}
