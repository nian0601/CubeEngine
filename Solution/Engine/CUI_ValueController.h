#pragma once
class CUI_ValueController
{
public:
	CUI_ValueController(float* aValue);

	bool UpdateValue();
	void AsString(CE_String& outString) const;

	bool IsValid() const { return myValuePtr != nullptr; }

private:
	float myCachedValue;
	float* myValuePtr;
};

