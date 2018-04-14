#pragma once

#include "CE_GrowingArray.h"

struct CE_SignatureElement
{
	bool operator==(const CE_SignatureElement& aOther)
	{
		return myParameterType == aOther.myParameterType &&
			mySemanticIndex == aOther.mySemanticIndex &&
			mySemanticNameIndex == aOther.mySemanticNameIndex;
	}

	bool operator!=(const CE_SignatureElement& aOther)
	{
		return !(*this == aOther);
	}

	int myParameterType;
	unsigned int mySemanticIndex;
	int mySemanticNameIndex;
};

struct CE_ShaderSignature
{
	bool operator==(const CE_ShaderSignature& aOther)
	{
		return myElements == aOther.myElements;
	}
	CE_GrowingArray<CE_SignatureElement> myElements;
};
