#include "stdafx.h"
#include "CUI_ValueController.h"
#include "CUI_Message.h"

CUI_ValueController::CUI_ValueController(float* aValue)
	: myValuePtr(aValue)
	, myHasCachedValue(false)
{
}

bool CUI_ValueController::UpdateValue()
{
	CE_ASSERT(myValuePtr != nullptr, "Invalid ValuePtr in CUI_ValueController");

	if (!myHasCachedValue || myCachedValue != *myValuePtr)
	{
		myCachedValue = *myValuePtr;
		myHasCachedValue = true;
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

float CUI_ValueController::Get() const
{
	return myCachedValue;
}

void CUI_ValueController::Set(float aValue)
{
	CE_ASSERT(myValuePtr != nullptr, "Invalid ValuePtr in CUI_ValueController");

	*myValuePtr = aValue;
}

void CUI_ValueController::OnScroll(const CUI_MouseMessage& aMessage)
{
	float currValue = myCachedValue;
	float speed = 0.01f;
	if (aMessage.ShiftDown())
		speed = 1.f;

	if (aMessage.myMouseWheelDelta > 0)
		currValue += speed;
	else
		currValue -= speed;

	Set(currValue);
}
