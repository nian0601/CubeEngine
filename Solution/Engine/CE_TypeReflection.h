#pragma once
#include "CE_Any.h"
#include "CE_TypeID.h"

class CE_TypeReflection
{
public:
	struct TypeInfo
	{
		const char* myName;
		CE_Vector4f myColor;
		CE_Any myDefaultValue;
	};

	template <typename T>
	static TypeInfo& GetTypeInfo()
	{
		bool isRegistered = IsRegistered<T>();
		CE_ASSERT(isRegistered, "Tried to GetTypeInfo of a unregistered type");

		if (isRegistered)
		{
			unsigned int typeID = CE_TypeID<CE_Type_Dummy_Parent>::GetID<T>();
			return myTypeInfo[typeID];
		}

		return myEmptyInfo;
	}

	static TypeInfo& GetTypeInfo(unsigned int anID)
	{
		CE_ASSERT(myTypeInfo.KeyExists(anID), "Tried to GetTypeInfo of an unrgesitered type");
		return myTypeInfo[anID];
	}

	template <typename T>
	static unsigned int GetID()
	{
		unsigned int typeID = CE_TypeID<CE_Type_Dummy_Parent>::GetID<T>();
		CE_ASSERT(myTypeInfo.KeyExists(typeID), "Tried to GetID of an unregistered type");

		return typeID;
	}

	template <typename T>
	static bool IsRegistered()
	{
		unsigned int typeID = CE_TypeID<CE_Type_Dummy_Parent>::GetID<T>();
		return myTypeInfo.KeyExists(typeID);
	}

	template <typename T>
	static void RegisterType(const char* aName, const CE_Vector4f& aColor)
	{
		CE_ASSERT(IsRegistered<T>() == false, "Tried to register type [%s] more than once", aName);

		unsigned int typeID = CE_TypeID<CE_Type_Dummy_Parent>::GetID<T>();
		TypeInfo& info = myTypeInfo[typeID];
		info.myName = aName;
		info.myColor = aColor;
		info.myDefaultValue.Set(T());
	}

	template <typename T>
	static void RegisterType(const char* aName)
	{
		RegisterType<T>(aName, CE_Vector4f(0.1f, 0.1f, 0.1f, 1.f));
	}

private:
	struct CE_Type_Dummy_Parent {};
	static TypeInfo myEmptyInfo;

	static CE_Map<unsigned int, TypeInfo> myTypeInfo;
};

template <typename T>
inline unsigned int CE_GetTypeID()
{
	return CE_TypeReflection::GetID<T>();
}

template <typename T>
inline bool CE_IsTypeValid()
{
	return CE_TypeReflection::IsRegistered<T>();
}

template <typename T>
inline const CE_TypeReflection::TypeInfo& CE_GetTypeInfo()
{
	return CE_TypeReflection::GetTypeInfo<T>();
}

inline const CE_TypeReflection::TypeInfo& CE_GetTypeInfo(unsigned int anID)
{
	return CE_TypeReflection::GetTypeInfo(anID);
}

#define CE_TYPE_REGISTER_NO_COLOR(type) CE_TypeReflection::RegisterType<type>(#type)
#define CE_TYPE_REGISTER(type, aColor) CE_TypeReflection::RegisterType<type>(#type, aColor)
