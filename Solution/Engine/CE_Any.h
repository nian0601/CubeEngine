#pragma once

#include "CE_TypeID.h"

class CE_Any
{
public:
	CE_Any()
		: myTypeID(static_cast<unsigned int>(-1))
#ifdef _DEBUG
		, myTypeName(nullptr)
#endif
	{}

	template <typename T>
	void Set(const T& someData);

	template <typename T>
	T& Get();

	template <typename T>
	bool Is();

	void Clear() { myTypeID = static_cast<unsigned int>(-1); myTypeName = nullptr; }
	bool IsEmpty() { return myTypeID == static_cast<unsigned int>(-1); }

	unsigned int GetSerializationSize() const { return sizeof(CE_Any) - sizeof(const char*); }

private:
	char myData[24];
	unsigned int myTypeID;

	// Dont add any members below this comment, as it will mess with serialization
	// if you add ANY new fields you have to make sure to update all existing savefiles as well...

	// Need to figure out a nice way of setting myTypeName when creating CE_Any from file.
#ifdef _DEBUG
	const char* myTypeName;
#endif
};

template <typename T>
void CE_Any::Set(const T& someData)
{
	CE_CT_ASSERT(sizeof(T) <= 24, "Trying to store a too large object by value in CE_Any. Theres only 24 available bytes");
	CE_ASSERT(CE_IsTypeValid<T>(), "CE_Any tried to use type that wasnt registered, thats not safe!");

	unsigned int typeID = CE_TypeID<CE_Any>::GetID<T>();
	myTypeID = typeID;

	*(T*)myData = someData;

#ifdef _DEBUG
	myTypeName = CE_GetTypeInfo<T>().myName;
#endif
}

template <typename T>
T& CE_Any::Get()
{
	unsigned int typeID = CE_TypeID<CE_Any>::GetID<T>();

#ifdef _DEBUG
	const char* requestedTypeName = CE_GetTypeInfo<T>().myName;
	CE_ASSERT(myTypeID == typeID, "CE_Any::Get called with missmatching types. Stored Type: %s, Requested Type: %s", myTypeName, requestedTypeName);
#else
	CE_ASSERT(myTypeID == typeID, "CE_Any::Get called with missmatching types");
#endif

	return *(T*)myData;
}

template <typename T>
bool CE_Any::Is()
{
	unsigned int typeID = CE_TypeID<CE_Any>::GetID<T>();
	return myTypeID == typeID;
}