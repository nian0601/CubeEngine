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

private:
	struct Data
	{
		char myData[24];
		unsigned int myTypeID;

#ifdef _DEBUG
		CE_String myTypeName;
#endif
	};

	CE_Map<CE_String, Data> myData;

	template <typename T>
	void AssertDataType(const CE_String& aName, const Data& someData);
};

template <typename T>
void CE_Blackboard::Set(const CE_String& aName, const T& someData)
{
	CE_ASSERT(sizeof(T) <= 24, "Trying to store a too large object by value in Blackboard. Theres only 24 available bytes");

	unsigned int typeID = CE_TypeID<Data>::GetID<T>();
	if (myData.KeyExists(aName) == false)
	{
		Data& data = myData[aName];
		data.myTypeID = typeID;

		//#ifdef _DEBUG
		//		data.myTypeName = CE_GET_TYPE_NAME(T);
		//#endif
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
	
	Data& data = myData[aName];
	AssertDataType<T>(aName, data);

	someData = *(T*)data.myData;

	return true;
}

template <typename T>
void CE_Blackboard::AssertDataType(const CE_String& aName, const Data& someData)
{
	unsigned int requestedTypeID = CE_TypeID<Data>::GetID<T>();
	aName;
	
	//#ifdef _DEBUG
	//	if (someData.myTypeID != requestedTypeID)
	//	{
	//		const char* requestedTypeName = CE_GET_TYPE_NAME(T);
	//		CE_ASSERT_ALWAYS("Missmatching datatype in Blackboard. Name: %s, Stored Type: %s, Requested Type: %s", aName.c_str(), someData.myTypeName.c_str(), requestedTypeName);
	//	}
	//#else
	//	CE_ASSERT(someData.myTypeID == requestedTypeID, "Missmatching typeID in blackboard");
	//#endif

	CE_ASSERT(someData.myTypeID == requestedTypeID, "Missmatching typeID in blackboard");
}