#pragma once

#include <cassert>
#include <stdlib.h>
#include <stdio.h>
#include "CE_Murmur.h"
#include <string.h>

#define CE_STRING_START_SIZE 20
#define CE_STRING_MAX_NUMBER_CHARACTERS 30

class CE_String
{
	friend class FileStream;
public:
	CE_String();
	~CE_String();
	CE_String(const CE_String &aString);
	CE_String(const char* aString);
	CE_String(const char aString);

	CE_String& operator+=(const CE_String &aString);
	CE_String& operator+=(const float aFloat);
	CE_String& operator+=(const double aDouble);
	CE_String& operator+=(const int aInt);
	CE_String& operator+=(const char* aString);
	CE_String& operator+=(const char aString);

	bool operator==(const CE_String &aString) const;
	void operator=(const CE_String &aString);
	void operator=(const char* aString);

	int Find(const char* aPattern, const int aStartIndex = 0) const;
	int Find(const CE_String &aPattern, const int aStartIndex = 0) const;
	int RFind(const char* aPattern) const;
	int RFind(const CE_String &aPattern) const;
	static const int NotFound = -1;
	static const unsigned char NullTermination = 0;

	CE_String SubStr(const int aStart, const int aEnd) const;
	const char* c_str() const;

	int Lenght() const;
	bool Empty() const;

	const char& operator[](const int aIndex) const;
	char& operator[](const int aIndex);

	void Resize(int aNewSize);
	unsigned int GetHash() const;

	void Clear();
	void RemoveOne();

private:
	void UpdateHash();

	int myCurrentSize;
	int myMaxSize;
	unsigned int myHash;
	char* myData;
};

inline CE_String::CE_String()
	: myCurrentSize(0)
	, myMaxSize(CE_STRING_START_SIZE)
	, myData(new char[CE_STRING_START_SIZE])
{
	myHash = 0;
}

inline CE_String::~CE_String()
{
	CE_SAFE_DELETE_ARRAY(myData);
}

inline CE_String::CE_String(const CE_String &aString)
	: myCurrentSize(0)
	, myMaxSize(CE_STRING_START_SIZE)
	, myData(new char[CE_STRING_START_SIZE])
{
	*this = aString;
}

inline CE_String::CE_String(const char* aString)
	: myCurrentSize(0)
	, myMaxSize(CE_STRING_START_SIZE)
	, myData(new char[CE_STRING_START_SIZE])
{
	*this = aString;
}

inline CE_String::CE_String(const char aChar)
	: myCurrentSize(0)
	, myMaxSize(CE_STRING_START_SIZE)
	, myData(new char[CE_STRING_START_SIZE])
{
	myHash = 0;
	myData[myCurrentSize] = aChar;
	++myCurrentSize;
	UpdateHash();
}

inline CE_String& CE_String::operator+=(const CE_String &aString)
{
	/*assert(myCurrentSize < MaxSize && "[String] Tried to add a String to an already full string.");
	assert(myCurrentSize + aString.Size() < MaxSize && "[String]: Tried to add to an string that would result in an overflow.");
	*/
	if (myCurrentSize + aString.Lenght() + 1 >= myMaxSize)
		Resize(myCurrentSize + aString.Lenght() + 1);

	for (int i = 0; i < aString.myCurrentSize; ++i)
	{
		myData[myCurrentSize] = aString[i];
		++myCurrentSize;
	}

	if (myData[myCurrentSize] != NullTermination)
	{
		myData[myCurrentSize] = NullTermination;
	}

	UpdateHash();

	return *this;
}

inline CE_String& CE_String::operator+=(const float aFloat)
{
	//assert(myCurrentSize < MaxSize && "[String] Tried to add a Float to an already full string.");

	if (myCurrentSize >= myMaxSize)
		Resize(myCurrentSize * 2);

	char buffer[CE_STRING_MAX_NUMBER_CHARACTERS];
	_snprintf_s(buffer, CE_STRING_MAX_NUMBER_CHARACTERS, "%f", aFloat);

	CE_String newString(buffer);
	*this += newString;

	return *this;
}

inline CE_String& CE_String::operator+=(const double aDouble)
{
	//assert(myCurrentSize < MaxSize && "[String] Tried to add a Double to an already full string.");


	if (myCurrentSize >= myMaxSize)
		Resize(myCurrentSize * 2);

	char buffer[CE_STRING_MAX_NUMBER_CHARACTERS];
	_snprintf_s(buffer, CE_STRING_MAX_NUMBER_CHARACTERS, "%f", aDouble);

	CE_String newString(buffer);
	*this += newString;

	return *this;
}

inline CE_String& CE_String::operator+=(const int aInt)
{
	//assert(myCurrentSize < MaxSize && "[String] Tried to add a Int to an already full string.");

	if (myCurrentSize >= myMaxSize)
		Resize(myCurrentSize * 2);

	char buffer[CE_STRING_MAX_NUMBER_CHARACTERS];
	_snprintf_s(buffer, CE_STRING_MAX_NUMBER_CHARACTERS, "%i", aInt);

	CE_String newString(buffer);
	*this += newString;

	return *this;
}

inline CE_String& CE_String::operator+=(const char* aString)
{
	//assert(myCurrentSize < MaxSize && "[String] Tried to add a Char* to an already full string.");

	if (myCurrentSize >= myMaxSize)
		Resize(myCurrentSize * 2);

	CE_String newString(aString);
	*this += newString;

	return *this;
}

inline CE_String& CE_String::operator+=(const char aChar)
{
	//assert(myCurrentSize < MaxSize && "[String] Tried to add a Char to an already full string.");

	if (myCurrentSize >= myMaxSize)
		Resize(myCurrentSize * 2);

	CE_String newString(aChar);
	*this += newString;

	return *this;
}


inline bool CE_String::operator==(const CE_String &aString) const
{
	return myHash == aString.myHash;

	/*if (aString.Size() != Size()) return false;

	for (int i = 0; i < aString.Size(); ++i)
	if (aString[i] != myData[i]) return false;

	return true;*/
}

inline void CE_String::operator=(const CE_String &aString)
{
	int targetLenght = aString.Lenght();
	if (myMaxSize <= targetLenght + 1)
		Resize(targetLenght + 1);
	//assert(aString.Size() < MaxSize && "[String] Tried to assign a larger string to a smaller one.");

	myCurrentSize = 0;
	for (int i = 0; i < aString.myCurrentSize; ++i)
	{
		myData[myCurrentSize] = aString[i];
		++myCurrentSize;
	}

	if (myData[myCurrentSize] != NullTermination)
	{
		myData[myCurrentSize] = NullTermination;
	}

	UpdateHash();
}

inline void CE_String::operator=(const char* aString)
{
	int targetLenght = static_cast<int>(strlen(aString));

	if (myMaxSize <= targetLenght)
		Resize(targetLenght);
	//assert(strlen(aString) < MaxSize && "[String]: Tried to assing a larget string to a smaller one.");

	myCurrentSize = 0;
	for (unsigned int i = 0; i < strlen(aString); ++i)
	{
		myData[myCurrentSize] = aString[i];
		++myCurrentSize;
	}

	if (myData[myCurrentSize] != NullTermination)
	{
		myData[myCurrentSize] = NullTermination;
	}

	UpdateHash();
}


inline int CE_String::Find(const char* aPattern, const int aStartIndex) const
{
	CE_String pattern(aPattern);

	return Find(pattern, aStartIndex);
}

inline int CE_String::Find(const CE_String &aPattern, const int aStartIndex) const
{
	int lenghtOfText = myCurrentSize;
	int lenghtOfPattern = aPattern.myCurrentSize;

	int jumpTable[256];
	for (int i = 0; i < 256; ++i)
	{
		jumpTable[i] = NotFound;
	}

	for (int k = 0; k < lenghtOfPattern - 1; ++k)
	{
		jumpTable[aPattern[k]] = lenghtOfPattern - k - 1;
	}


	if (lenghtOfPattern > lenghtOfText) return NotFound;

	int k = lenghtOfPattern - 1 + aStartIndex;

	while (k < lenghtOfText)
	{
		int j = lenghtOfPattern - 1;
		int i = k;
		while (j >= 0 && myData[i] == aPattern[j])
		{
			--j;
			--i;
		}
		if (j == -1)
			return i + 1;


		int jumpValue = jumpTable[myData[k]];
		if (jumpValue == NotFound)
		{
			k += lenghtOfPattern;
		}
		else
		{
			k += jumpValue;
		}
	}


	return NotFound;
}

inline int CE_String::RFind(const char* aPattern) const
{
	CE_String pattern(aPattern);

	return RFind(pattern);
}

inline int CE_String::RFind(const CE_String &aPattern) const
{
	int lenghtOfText = myCurrentSize;
	int lenghtOfPattern = aPattern.myCurrentSize;

	int jumpTable[256];
	for (int x = 0; x < 256; ++x)
	{
		jumpTable[x] = NotFound;
	}


	for (int x = lenghtOfPattern - 1; x > 0; --x)
	{
		jumpTable[aPattern[x]] = x;
	}

	if (lenghtOfPattern > lenghtOfText) return NotFound;

	int k = myCurrentSize - 1;

	while (k >= 0)
	{
		int j = 0;
		int i = k;
		while (j <= lenghtOfPattern /*- 1*/ && myData[i] == aPattern[j])
		{
			++j;
			++i;
		}
		if (j == lenghtOfPattern)
			return i - lenghtOfPattern;


		int jumpValue = jumpTable[myData[k]];
		if (jumpValue == NotFound)
		{
			k -= lenghtOfPattern;
		}
		else
		{
			k -= jumpValue;
		}
	}


	return NotFound;
}


inline CE_String CE_String::SubStr(const int aStart, const int aEnd) const
{
	assert(aStart < myMaxSize && "[String]: Tried to create a SubString starting at an Index greater than the lenght of the main-string.");
	assert(aStart >= 0 && "[String]: Tried to create a SubString starting at an Index greater than the lenght of the main-string.");
	assert(aEnd <= Lenght() && "[String]: Tried to create a SubString thats longer than the main-string.");

	CE_String newString;
	for (int i = aStart; i <= aEnd; ++i)
	{
		newString += static_cast<char>(operator[](i));
	}


	if (newString[newString.myCurrentSize] != NullTermination)
	{
		newString[newString.myCurrentSize] = NullTermination;
	}

	return newString;
}

inline const char* CE_String::c_str() const
{
	return myData;
}

inline int CE_String::Lenght() const
{
	return myCurrentSize - 1;
}

inline bool CE_String::Empty() const
{
	return myCurrentSize == 0;
}

inline const char& CE_String::operator[](const int aIndex) const
{
	assert(aIndex < myCurrentSize + 1 && "[String]: Tried to use Subscript with an index greater than the lenght.");
	assert(aIndex >= 0 && "[Sting]: Tried to use Subscript with an index smaller than 0");

	return myData[aIndex];
}

inline char& CE_String::operator[](const int aIndex)
{
	assert(aIndex < myCurrentSize + 1 && "[String]: Tried to use Subscript with an index greater than the lenght.");
	assert(aIndex >= 0 && "[Sting]: Tried to use Subscript with an index smaller than 0");

	return myData[aIndex];
}

inline void CE_String::Resize(int aNewSize)
{
	assert(aNewSize > 0 && "Can't make String smaller than 1.");

	myMaxSize = aNewSize + 1;
	char* newData = new char[myMaxSize];
	//memcpy(newData, myData, sizeof(char) * myCurrentSize);
	for (int i = 0; i < myCurrentSize; ++i)
	{
		newData[i] = myData[i];
	}

	newData[myCurrentSize] = NullTermination;

	delete[] myData;
	myData = newData;
}

inline unsigned int CE_String::GetHash() const
{
	return myHash;
}

inline void CE_String::Clear()
{
	for (int i = 0; i < myCurrentSize; ++i)
		myData[i] = NullTermination;

	myCurrentSize = 0;
}

inline void CE_String::RemoveOne()
{
	if (myCurrentSize <= 0)
		return;

	myCurrentSize -= 1;
}

inline void CE_String::UpdateHash()
{
	MurmurHash3_x86_32(myData, myCurrentSize, 1000, &myHash);
}

//
//
//

inline CE_String operator+(const CE_String &aFirst, const CE_String &aSecond)
{
	CE_String newString(aFirst);
	newString += aSecond;

	return newString;
}

inline CE_String operator+(const CE_String &aString, const float aFloat)
{
	CE_String newString(aString);
	newString += aFloat;

	return newString;
}

inline CE_String operator+(const CE_String &aString, const double aDouble)
{
	CE_String newString(aString);
	newString += aDouble;

	return newString;
}

inline CE_String operator+(const CE_String &aString, const int aInt)
{
	CE_String newString(aString);
	newString += aInt;

	return newString;
}

inline CE_String operator+(const CE_String &aFirst, const char* aSecond)
{
	CE_String newString(aFirst);
	newString += aSecond;

	return newString;
}

inline CE_String operator+(const CE_String &aString, const char aChar)
{
	CE_String newString(aString);
	newString += aChar;

	return newString;
}

//

inline CE_String operator+(const float aFloat, const CE_String &aString)
{
	CE_String newString(aString);
	newString += aFloat;

	return newString;
}

inline CE_String operator+(const double aDouble, const CE_String &aString)
{
	CE_String newString(aString);
	newString += aDouble;

	return newString;
}

inline CE_String operator+(const int aInt, const CE_String &aString)
{
	CE_String newString(aString);
	newString += aInt;

	return newString;
}

inline CE_String operator+(const char* aSecond, const CE_String &aFirst)
{
	CE_String newString(aFirst);
	newString += aSecond;

	return newString;
}

inline CE_String operator+(const char aChar, const CE_String &aString)
{
	CE_String newString(aString);
	newString += aChar;

	return newString;
}