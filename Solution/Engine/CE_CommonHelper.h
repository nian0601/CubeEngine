#pragma once

inline CE_String CE_ReplaceExtension(const CE_String& aFilePath, const CE_String& aNewExtension)
{
	CE_String newString = aFilePath;
	int extensionIndex = newString.RFind(".");
	newString = newString.SubStr(0, extensionIndex);

	newString += aNewExtension;

	return newString;
}