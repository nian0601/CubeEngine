#pragma once
#include "CE_CustomEntityTypes.h"
#include "CE_TypeReflection.h"

template <typename... Args>
struct CE_TypeList { };

template <typename... Args>
struct CE_Requires : CE_TypeList<Args...> {};

template <typename... Args>
struct CE_Excludes : CE_TypeList<Args...> {};

template<typename... Args>
static void CE_Types(unsigned int&, CE_TypeList<Args...>) {}


template<typename T, typename... Args>
static void CE_Types(unsigned int& someFlags, CE_TypeList<T, Args...>)
{
	unsigned int componentID = CE_TypeID<CE_BaseComponent>::GetID<T>();
	someFlags |= (1 << componentID);

	CE_Types(someFlags, CE_TypeList<Args...>());
}

struct CE_ComponentFilter
{
	CE_ComponentFilter()
		: myRequiresFlags(0)
		, myExcludesFlags(0)
	{}

	CE_ComponentFilter(unsigned int someRequires, unsigned int someExcludes)
		: myRequiresFlags(someRequires)
		, myExcludesFlags(someExcludes)
	{}

	bool Compare(unsigned int someComponentFlags) const
	{
		if ((someComponentFlags & myRequiresFlags) != myRequiresFlags)
			return false;

		if ((someComponentFlags & myExcludesFlags) > 0)
			return false;

		return true;
	}

private:
	unsigned int myRequiresFlags;
	unsigned int myExcludesFlags;
};

template <typename RequireList = CE_Requires<>, typename ExcludeList = CE_Excludes<>>
CE_ComponentFilter CE_CreateFilter()
{
	unsigned int requires = 0;
	unsigned int excludes = 0;

	CE_Types(requires, RequireList{});
	CE_Types(excludes, ExcludeList{});

	return CE_ComponentFilter(requires, excludes);
}