#pragma once

#include "CE_TypeID.h"
#include "CE_Map.h"
#include "CE_String.h"

class CE_Blackboard
{
public:
	template <typename T>
	void Set(const CE_String& aName, const T& someData);

	template <typename T>
	bool Get(const CE_String& aName, T& someData);

	template <typename T>
	T Get(const CE_String& aName);

private:
	struct Data
	{
		char myData[24];
		unsigned int myTypeID;

#ifdef _DEBUG
		const char* myTypeName;
#endif
	};

	CE_Map<CE_String, Data> myData;

	template <typename T>
	void AssertDataType(const CE_String& aName, const Data& someData);
};

template <typename T>
void CE_Blackboard::Set(const CE_String& aName, const T& someData)
{
	CE_CT_ASSERT(sizeof(T) <= 24, "Trying to store a too large object by value in Blackboard. Theres only 24 available bytes");
	CE_ASSERT(CE_IsTypeValid<T>(), "Blackboard tried to use type that wasnt registered, thats not safe!");

	unsigned int typeID = CE_TypeID<Data>::GetID<T>();
	if (myData.KeyExists(aName) == false)
	{
		Data& data = myData[aName];
		data.myTypeID = typeID;

		#ifdef _DEBUG
			data.myTypeName = CE_GetTypeInfo<T>().myName;
		#endif
	}

	Data& data = myData[aName];
	AssertDataType<T>(aName, data);

	*(T*)data.myData = someData;
}

template <typename T>
bool CE_Blackboard::Get(const CE_String& aName, T& someData)
{
	if (myData.KeyExists(aName) == false)
		return false;
	
	CE_ASSERT(CE_IsTypeValid<T>(), "Blackboard tried to use type that wasnt registered, thats not safe!");

	Data& data = myData[aName];
	AssertDataType<T>(aName, data);

	someData = *(T*)data.myData;

	return true;
}

template <typename T>
T CE_Blackboard::Get(const CE_String& aName)
{
	CE_ASSERT(myData.KeyExists(aName), "Blackboard tried to get variable [%s] that wasnt set.", aName.c_str());
	CE_ASSERT(CE_IsTypeValid<T>(), "Blackboard tried to use type that wasnt registered, thats not safe!");

	Data& data = myData[aName];
	AssertDataType<T>(aName, data);

	return *(T*)data.myData;
}

template <typename T>
void CE_Blackboard::AssertDataType(const CE_String& aName, const Data& someData)
{
	unsigned int requestedTypeID = CE_TypeID<Data>::GetID<T>();
	
	#ifdef _DEBUG
	if (someData.myTypeID != requestedTypeID)
	{
		const char* requestedTypeName = CE_GetTypeInfo<T>().myName;
		CE_ASSERT_ALWAYS("Missmatching datatype in Blackboard. Name: %s, Stored Type: %s, Requested Type: %s", aName.c_str(), someData.myTypeName, requestedTypeName);
	}
	#else
	aName;
	CE_ASSERT(someData.myTypeID == requestedTypeID, "Missmatching typeID in blackboard");
	#endif
}