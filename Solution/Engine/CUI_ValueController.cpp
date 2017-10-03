#include "stdafx.h"
#include "CUI_ValueController.h"

CUI_ValueController::CUI_ValueController(float* aValue)
	: myValuePtr(aValue)
{
}

bool CUI_ValueController::UpdateValue()
{
	CE_ASSERT(myValuePtr != nullptr, "Invalid ValuePtr in CUI_ValueController");

	if (myCachedValue != *myValuePtr)
	{
		myCachedValue = *myValuePtr;
		return true;
	}

	return false;
}

void CUI_ValueController::AsString(CE_String& outString) const
{
	CE_ASSERT(myValuePtr != nullptr, "Invalid ValuePtr in CUI_ValueController");

	outString += myCachedValue;
	outString += " ";
}