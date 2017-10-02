#pragma once

inline CE_String CE_ReplaceExtension(const CE_String& aFilePath, const CE_String& aNewExtension)
{
	CE_String newString = aFilePath;
	int extensionIndex = newString.RFind(".");
	newString = newString.SubStr(0, extensionIndex);

	newString += aNewExtension;

	return newString;
}

inline float CE_RoundToInt(float aFloat)
{
	return (float)(int)(aFloat + 0.5f);
}

template <typename T>
inline T CE_Min(const T& aFirst, const T& aSecond)
{
	return aFirst < aSecond ? aFirst : aSecond;
}

template <typename T>
inline T CE_Max(const T& aFirst, const T& aSecond)
{
	return aFirst > aSecond ? aFirst : aSecond;
}