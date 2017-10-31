#pragma once
#include "CE_Map.h"

template <typename BaseType>
class CE_TypeID
{
public:
	template <typename T>
	static unsigned int GetID()
	{
		static const unsigned int id = myNextID++;
		return id;
	}
private:
	static unsigned int myNextID;
};

template <typename BaseType>
unsigned int CE_TypeID<BaseType>::myNextID = 0;


class CE_TypeReflection
{
public:
	template <typename T>
	static const char* GetName()
	{
		bool isRegistered = IsRegistered<T>();
		CE_ASSERT(isRegistered, "Tried to GetName() of a unregistered type");
		
		if (isRegistered)
		{
			unsigned int typeID = CE_TypeID<CE_Type_Dummy_Parent>::GetID<T>();
			return myNames[typeID];
		}

		return nullptr;
	}

	template <typename T>
	static bool IsRegistered()
	{
		unsigned int typeID = CE_TypeID<CE_Type_Dummy_Parent>::GetID<T>();
		return myNames.KeyExists(typeID);
	}

	template <typename T>
	static void RegisterType(const char* aName)
	{
		CE_ASSERT(IsRegistered<T>() == false, "Tried to register type [%s] more than once", aName);

		unsigned int typeID = CE_TypeID<CE_Type_Dummy_Parent>::GetID<T>();
		myNames[typeID] = aName;
	}

private:
	struct CE_Type_Dummy_Parent {};

	static CE_Map<unsigned int, const char*> myNames;
};

#define CE_TYPE_GET_NAME(type) CE_TypeReflection::GetName<type>()
#define CE_TYPE_IS_REGISTERED(type) CE_TypeReflection::IsRegistered<type>()
#define CE_TYPE_REGISTER(type) CE_TypeReflection::RegisterType<type>(#type)