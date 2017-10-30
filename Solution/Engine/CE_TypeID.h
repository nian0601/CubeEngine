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

	template <typename T>
	static const char* GetName(const char* aName)
	{
		static const char* name = aName;
		return name;
	}

private:
	static unsigned int myNextID;
};

template <typename BaseType>
unsigned int CE_TypeID<BaseType>::myNextID = 0;


struct CE_Dummy_Type_Parent
{};

class CE_TypeReflection
{
public:
	template <typename T>
	static const char* GetName()
	{
		unsigned int typeID = CE_TypeID<CE_Dummy_Type_Parent>::GetID<T>();
		return myNames[typeID];
	}

	static void RegisterType(unsigned int anID, const char* aName)
	{
		myNames[anID] = aName;
	}

private:
	static CE_Map<unsigned int, const char*> myNames;
};




#define CE_GET_TYPE_NAME(type) CE_TypeReflection::GetName<type>()
#define CE_REGISTER_TYPE(type) CE_TypeReflection::RegisterType(CE_TypeID<CE_Dummy_Type_Parent>::GetID<type>(), #type)