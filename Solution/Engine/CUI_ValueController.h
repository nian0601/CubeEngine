#pragma once

struct CUI_MouseMessage;
class CUI_ValueController
{
public:
	CUI_ValueController(float* aValue);

	bool UpdateValue();
	void AsString(CE_String& outString) const;

	bool IsValid() const { return myValuePtr != nullptr; }

	float Get() const;
	void Set(float aValue);

	void OnScroll(const CUI_MouseMessage& aMessage);

private:
	float myCachedValue;
	float* myValuePtr;
	bool myHasCachedValue;
};

