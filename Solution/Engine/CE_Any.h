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

private:
	char myData[24];
	unsigned int myTypeID;

#ifdef _DEBUG
	const char* myTypeName;
#endif
};

template <typename T>
void CE_Any::Set(const T& someData)
{
	CE_CT_ASSERT(sizeof(T) <= 24, "Trying to store a too large object by value in CE_Any. Theres only 24 available bytes");
	CE_ASSERT(CE_TYPE_IS_REGISTERED(T), "CE_Any tried to use type that wasnt registered, thats not safe!");

	unsigned int typeID = CE_TypeID<CE_Any>::GetID<T>();
	myTypeID = typeID;

	*(T*)myData = someData;

#ifdef _DEBUG
	myTypeName = CE_TYPE_GET_NAME(T);
#endif
}

template <typename T>
T& CE_Any::Get()
{
	unsigned int typeID = CE_TypeID<CE_Any>::GetID<T>();

#ifdef _DEBUG
	const char* requestedTypeName = CE_TYPE_GET_NAME(T);
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