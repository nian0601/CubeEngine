#pragma once
#include "CE_CustomEntityTypes.h"
#include "CE_TypeID.h"

template <typename... Args>
struct TypeList { };

template <typename... Args>
struct Requires : TypeList<Args...> {};

template <typename... Args>
struct Excludes : TypeList<Args...> {};


template<typename... Args>
static void Types(CE_EntityComponentArray&, TypeList<Args...>) {}


template<typename T, typename... Args>
static void Types(CE_EntityComponentArray& someComponents, TypeList<T, Args...>)
{
	someComponents[CE_TypeID<CE_BaseComponent>::GetID<T>()] = 1;
	Types(someComponents, TypeList<Args...>());
}

struct CE_ComponentFilter
{
	CE_ComponentFilter()
	{
		for (int i = 0; i < MAX_NUMBER_OF_COMPONENTS; ++i)
		{
			myRequires[i] = 0;
			myExcludes[i] = 0;
		}
	}

	CE_ComponentFilter(const CE_EntityComponentArray& someRequires
		, const CE_EntityComponentArray& someExludes)
		: myRequires(someRequires)
		, myExcludes(someExludes)
	{}

	bool Compare(const CE_EntityComponentArray& someComponents) const
	{
		for (int i = 0; i < MAX_NUMBER_OF_COMPONENTS; ++i)
		{
			if (myRequires[i] == 1 && someComponents[i] == -1)
			{
				return false;
			}

			if (myExcludes[i] == 1 && someComponents[i] != -1)
			{
				return false;
			}
		}

		return true;
	}

private:
	CE_EntityComponentArray myRequires;
	CE_EntityComponentArray myExcludes;
};



template <typename RequireList, typename ExcludeList = Excludes<>>
CE_ComponentFilter CreateFilter()
{
	CE_EntityComponentArray requires;
	CE_EntityComponentArray excludes;

	for (int i = 0; i < MAX_NUMBER_OF_COMPONENTS; ++i)
	{
		requires[i] = 0;
		excludes[i] = 0;
	}

	Types(requires, RequireList{});
	Types(excludes, ExcludeList{});

	return CE_ComponentFilter(requires, excludes);
}