#pragma once
#include "CE_Assert.h"

enum eGrowingArray_Errors
{
	INVALID_SIZE,
	LOW_INDEX,
	HIGH_INDEX,
	_GROWINGARRAY_ERROR_COUNT,
};

#ifdef CE_ARRAY_BOUNDS_CHECK
static const char* locGrowingArray_ErrorStrings[_GROWINGARRAY_ERROR_COUNT] = {
	"Invalid size of growingarray",
	"Index has to be 0 or more.",
	"a index out of bounds!"
};
#endif

template<typename ObjectType>
class CE_GrowingArray
{
public:
	CE_GrowingArray();
	CE_GrowingArray(const CE_GrowingArray& aGrowingArray);
	~CE_GrowingArray();

	CE_GrowingArray& operator=(const CE_GrowingArray& aGrowingArray);


	inline void Respace(int aNewSize);
	void Reserve(int aNrOfItems);

	inline ObjectType& operator[](const int& aIndex);
	inline const ObjectType& operator[](const int& aIndex) const;

	inline void Add(const ObjectType& aObject);
	inline ObjectType& Add();
	inline void AddEmptyObject();
	inline bool AddUnique(const ObjectType& aObject);
	inline void Insert(int aIndex, const ObjectType& aObject);
	inline void InsertFirst(const ObjectType& aObject);
	inline void DeleteCyclic(ObjectType& aObject);
	inline void DeleteCyclicAtIndex(int aItemNumber);
	inline void DeleteNonCyclicAtIndex(int aItemNumber);
	inline void RemoveCyclic(const ObjectType& aObject);
	inline void RemoveCyclicAtIndex(int aItemNumber);
	inline void RemoveNonCyclic(const ObjectType& aObject);
	inline void RemoveNonCyclicAtIndex(int aItemNumber);
	inline int Find(const ObjectType& aObject) const;

	inline ObjectType& GetLast();
	inline const ObjectType& GetLast() const;

	static const int FoundNone = -1;

	inline void RemoveAll();
	inline void DeleteAll();

	void Optimize();
	__forceinline int Size() const;
	__forceinline int GetCapacity() const;

	inline ObjectType* GetArrayAsPointer();

	typedef ObjectType* iterator;
	typedef const ObjectType* const_iterator;
	iterator begin() { return &myData[0]; }
	const_iterator begin() const { return &myData[0]; }
	iterator end() { return &myData[myCurrentSize]; }
	const_iterator end() const { return &myData[myCurrentSize]; }

private:
	ObjectType* myData;
	int myCurrentSize;
	int myMaxSize;
	bool myUseSafeModeFlag;
};

template<typename ObjectType>
inline CE_GrowingArray<ObjectType>::CE_GrowingArray()
	: myData(nullptr)
	, myCurrentSize(0)
	, myMaxSize(0)
	, myUseSafeModeFlag(true)
{
	Respace(1);
}

template<typename ObjectType>
inline CE_GrowingArray<ObjectType>::CE_GrowingArray(const CE_GrowingArray& aGrowingArray)
{
	myData = nullptr;
	operator=(aGrowingArray);
}

template<typename ObjectType>
inline CE_GrowingArray<ObjectType>::~CE_GrowingArray()
{
	delete[] myData;
}

template<typename ObjectType>
inline CE_GrowingArray<ObjectType>& CE_GrowingArray<ObjectType>::operator=(const CE_GrowingArray& aGrowingArray)
{
	// if aGrowingArray.myMaxSize <= myMaxSize, I could just copy everything, don't need newData
	delete[] myData;
	myMaxSize = aGrowingArray.myMaxSize;
	myCurrentSize = aGrowingArray.myCurrentSize;
	myUseSafeModeFlag = aGrowingArray.myUseSafeModeFlag;

	ObjectType* newData = new ObjectType[aGrowingArray.myMaxSize];


	if (myUseSafeModeFlag == true)
	{
		for (int i = 0; i < myCurrentSize; ++i)
		{
			newData[i] = aGrowingArray.myData[i];
		}
	}
	else
	{
		memcpy(newData, aGrowingArray.myData, sizeof(ObjectType) * aGrowingArray.myCurrentSize);
	}

	myData = newData;

	return *this;
}

template<typename ObjectType>
inline void CE_GrowingArray<ObjectType>::Respace(int aNewSize)
{
#ifdef CE_ARRAY_BOUNDS_CHECK
	CE_ASSERT(aNewSize > 0, locGrowingArray_ErrorStrings[INVALID_SIZE]);
#endif

	ObjectType* newData = new ObjectType[aNewSize];
	if (myUseSafeModeFlag == true)
	{
		for (int i = 0; i < myCurrentSize; ++i)
		{
			newData[i] = myData[i];
		}
	}
	else
	{
		memcpy(newData, myData, sizeof(ObjectType) * myCurrentSize);
	}
	delete[] myData;
	myData = newData;
	myMaxSize = static_cast<int>(aNewSize);
}

template<typename ObjectType>
inline void CE_GrowingArray<ObjectType>::Reserve(int aNrOfItems)
{
	Respace(aNrOfItems);
	myCurrentSize = aNrOfItems;
}

template<typename ObjectType>
inline ObjectType& CE_GrowingArray<ObjectType>::operator[](const int& aIndex)
{
#ifdef CE_ARRAY_BOUNDS_CHECK
	CE_ASSERT(aIndex >= 0, locGrowingArray_ErrorStrings[LOW_INDEX]);
	CE_ASSERT(aIndex < myCurrentSize, locGrowingArray_ErrorStrings[HIGH_INDEX]);
#endif

	return myData[aIndex];
}

template<typename ObjectType>
inline const ObjectType& CE_GrowingArray<ObjectType>::operator[](const int& aIndex) const
{
#ifdef CE_ARRAY_BOUNDS_CHECK
	CE_ASSERT(aIndex >= 0, locGrowingArray_ErrorStrings[LOW_INDEX]);
	CE_ASSERT(aIndex < myCurrentSize, locGrowingArray_ErrorStrings[HIGH_INDEX]);
#endif

	return myData[aIndex];
}

template<typename ObjectType>
inline void CE_GrowingArray<ObjectType>::Add(const ObjectType& aObject)
{
	if (myCurrentSize >= myMaxSize)
	{
		Respace(myMaxSize * 2);
	}
	myData[myCurrentSize++] = aObject;
}


template<typename ObjectType>
inline ObjectType& CE_GrowingArray<ObjectType>::Add()
{
	if (myCurrentSize >= myMaxSize)
	{
		Respace(myMaxSize * 2);
	}

	ObjectType& object = myData[myCurrentSize++];
	return object;
}


template<typename ObjectType>
inline void CE_GrowingArray<ObjectType>::AddEmptyObject()
{
	if (myCurrentSize == myMaxSize)
	{
		Respace(myMaxSize * 2);
	}

	myCurrentSize++;
}

template<typename ObjectType>
inline bool CE_GrowingArray<ObjectType>::AddUnique(const ObjectType& aObject)
{
	if (Find(aObject) == -1)
	{
		Add(aObject);
		return true;
	}

	return false;
}

template<typename ObjectType>
inline void CE_GrowingArray<ObjectType>::Insert(int aIndex, const ObjectType& aObject)
{
#ifdef CE_ARRAY_BOUNDS_CHECK
	CE_ASSERT(aIndex >= 0, locGrowingArray_ErrorStrings[LOW_INDEX]);
	CE_ASSERT(aIndex < myCurrentSize, locGrowingArray_ErrorStrings[HIGH_INDEX]);
#endif

	if (myCurrentSize >= myMaxSize)
	{
		Respace(myMaxSize * 2);
	}
	for (int i = myCurrentSize - 1; i >= aIndex; --i)
	{
		if (i == int(-1))
		{
			break;
		}
		myData[i + 1] = myData[i];
	}
	myData[aIndex] = aObject;
	++myCurrentSize;
}

template<typename ObjectType>
inline void CE_GrowingArray<ObjectType>::InsertFirst(const ObjectType& aObject)
{
	if (myCurrentSize == 0)
	{
		Add(aObject);
	}
	else
	{
		Insert(0, aObject);
	}
}

template<typename ObjectType>
inline void CE_GrowingArray<ObjectType>::DeleteCyclic(ObjectType& aObject)
{
	for (int i = 0; i < myCurrentSize; ++i)
	{
		if (myData[i] == aObject)
		{
			DeleteCyclicAtIndex(i);
			return;
		}
	}
	CE_ASSERT_ALWAYS("Object not found.");
}

template<typename ObjectType>
inline void CE_GrowingArray<ObjectType>::DeleteCyclicAtIndex(int aItemNumber)
{
#ifdef CE_ARRAY_BOUNDS_CHECK
	CE_ASSERT(aItemNumber >= 0, locGrowingArray_ErrorStrings[LOW_INDEX]);
	CE_ASSERT(aItemNumber < myCurrentSize, locGrowingArray_ErrorStrings[HIGH_INDEX]);
#endif

	delete myData[aItemNumber];
	myData[aItemNumber] = nullptr;
	myData[aItemNumber] = myData[--myCurrentSize];
}

template<typename ObjectType>
inline void CE_GrowingArray<ObjectType>::DeleteNonCyclicAtIndex(int aItemNumber)
{
#ifdef CE_ARRAY_BOUNDS_CHECK
	CE_ASSERT(aItemNumber >= 0, locGrowingArray_ErrorStrings[LOW_INDEX]);
	CE_ASSERT(aItemNumber < myCurrentSize, locGrowingArray_ErrorStrings[HIGH_INDEX]);
#endif

	delete myData[aItemNumber];
	myData[aItemNumber] = nullptr;

	for (int i = aItemNumber; i < myCurrentSize - 1; ++i)
	{
		myData[i] = myData[i + 1];
	}
	--myCurrentSize;
}


template<typename ObjectType>
inline void CE_GrowingArray<ObjectType>::RemoveCyclic(const ObjectType& aObject)
{
	for (int i = 0; i < myCurrentSize; ++i)
	{
		if (myData[i] == aObject)
		{
			RemoveCyclicAtIndex(i);
			return;
		}
	}
	CE_ASSERT_ALWAYS("Object not found.");
}

template<typename ObjectType>
inline void CE_GrowingArray<ObjectType>::RemoveCyclicAtIndex(int aItemNumber)
{
#ifdef CE_ARRAY_BOUNDS_CHECK
	CE_ASSERT(aItemNumber >= 0, locGrowingArray_ErrorStrings[LOW_INDEX]);
	CE_ASSERT(aItemNumber < myCurrentSize, locGrowingArray_ErrorStrings[HIGH_INDEX]);
#endif

	myData[aItemNumber] = myData[--myCurrentSize];
}

template<typename ObjectType>
inline void CE_GrowingArray<ObjectType>::RemoveNonCyclic(const ObjectType& aObject)
{
	for (int i = 0; i < myCurrentSize; ++i)
	{
		if (myData[i] == aObject)
		{
			RemoveNonCyclicAtIndex(i);
			return;
		}
	}
	CE_ASSERT_ALWAYS("Object not found.");
}


template<typename ObjectType>
inline void CE_GrowingArray<ObjectType>::RemoveNonCyclicAtIndex(int aItemNumber)
{
#ifdef CE_ARRAY_BOUNDS_CHECK
	CE_ASSERT(aItemNumber >= 0, locGrowingArray_ErrorStrings[LOW_INDEX]);
	CE_ASSERT(aItemNumber < myCurrentSize, locGrowingArray_ErrorStrings[HIGH_INDEX]);
#endif

	for (int i = aItemNumber; i < myCurrentSize - 1; ++i)
	{
		myData[i] = myData[i + 1];
	}
	--myCurrentSize;
}

template<typename ObjectType>
inline int CE_GrowingArray<ObjectType>::Find(const ObjectType& aObject) const
{
	for (int i = 0; i < myCurrentSize; ++i)
	{
		if (myData[i] == aObject)
		{
			return i;
		}
	}
	return FoundNone;
}

template<typename ObjectType>
inline ObjectType& CE_GrowingArray<ObjectType>::GetLast()
{
	return myData[myCurrentSize - 1];
}

template<typename ObjectType>
inline const ObjectType& CE_GrowingArray<ObjectType>::GetLast() const
{
	return myData[myCurrentSize - 1];
}

template<typename ObjectType>
inline void CE_GrowingArray<ObjectType>::RemoveAll()
{
	myCurrentSize = 0;
}

template<typename ObjectType>
inline void CE_GrowingArray<ObjectType>::DeleteAll()
{
	for (int i = 0; i < myCurrentSize; ++i)
	{
		delete myData[i];
		myData[i] = nullptr;
	}
	myCurrentSize = 0;
}

template<typename ObjectType>
inline void CE_GrowingArray<ObjectType>::Optimize()
{
	myMaxSize = myCurrentSize;
	if (myMaxSize < 1)
	{
		myMaxSize = 1;
	}
	Respace(myMaxSize);
}

template<typename ObjectType>
__forceinline int CE_GrowingArray<ObjectType>::Size() const
{
	return myCurrentSize;
}

template<typename ObjectType>
__forceinline int CE_GrowingArray<ObjectType>::GetCapacity() const
{
	return myMaxSize;
}

template<typename ObjectType>
inline ObjectType* CE_GrowingArray<ObjectType>::GetArrayAsPointer()
{
	return myData;
}