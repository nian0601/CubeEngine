#pragma once
#include <cstring>

enum eGrowingArray_Errors
{
	INVALID_SIZE,
	LOW_INDEX,
	HIGH_INDEX,
	_GROWINGARRAY_ERROR_COUNT,
};

static const char* locGrowingArray_ErrorStrings[_GROWINGARRAY_ERROR_COUNT] = {
	"Invalid size of growingarray",
	"Index has to be 0 or more.",
	"a index out of bounds!"
};

template<typename ObjectType, typename SizeType = int>
class CE_GrowingArray
{
public:
	CE_GrowingArray();
	CE_GrowingArray(const CE_GrowingArray& aGrowingArray);
	~CE_GrowingArray();

	CE_GrowingArray& operator=(const CE_GrowingArray& aGrowingArray);


	inline void Respace(int aNewSize);
	void Reserve(SizeType aNrOfItems, bool aUseSafeModeFlag = true);

	inline ObjectType& operator[](const SizeType& aIndex);
	inline const ObjectType& operator[](const SizeType& aIndex) const;

	inline void Add(const ObjectType& aObject);
	inline ObjectType& Add();
	inline void AddEmptyObject();
	inline bool AddUnique(const ObjectType& aObject);
	inline void Insert(SizeType aIndex, const ObjectType& aObject);
	inline void InsertFirst(const ObjectType& aObject);
	inline void DeleteCyclic(ObjectType& aObject);
	inline void DeleteCyclicAtIndex(SizeType aItemNumber);
	inline void DeleteNonCyclicAtIndex(SizeType aItemNumber);
	inline void RemoveCyclic(const ObjectType& aObject);
	inline void RemoveCyclicAtIndex(SizeType aItemNumber);
	inline void RemoveNonCyclic(const ObjectType& aObject);
	inline void RemoveNonCyclicAtIndex(SizeType aItemNumber);
	inline SizeType Find(const ObjectType& aObject) const;

	inline ObjectType& GetLast();
	inline const ObjectType& GetLast() const;

	static const SizeType FoundNone = static_cast<SizeType>(-1);

	inline void RemoveAll();
	inline void DeleteAll();

	void Optimize();
	__forceinline SizeType Size() const;
	__forceinline SizeType GetCapacity() const;

	inline ObjectType* GetArrayAsPointer();

	typedef ObjectType* iterator;
	typedef const ObjectType* const_iterator;
	iterator begin() { return &myData[0]; }
	const_iterator begin() const { return &myData[0]; }
	iterator end() { return &myData[myCurrentSize]; }
	const_iterator end() const { return &myData[myCurrentSize]; }

private:
	ObjectType* myData;
	SizeType myCurrentSize;
	SizeType myMaxSize;
	bool myUseSafeModeFlag;
};

template<typename ObjectType, typename SizeType = int>
inline CE_GrowingArray<ObjectType, SizeType>::CE_GrowingArray()
	: myData(nullptr)
	, myCurrentSize(0)
	, myMaxSize(0)
	, myUseSafeModeFlag(true)
{
	Respace(1);
}

template<typename ObjectType, typename SizeType = int>
inline CE_GrowingArray<ObjectType, SizeType>::CE_GrowingArray(const CE_GrowingArray& aGrowingArray)
{
	myData = nullptr;
	operator=(aGrowingArray);
}

template<typename ObjectType, typename SizeType = int>
inline CE_GrowingArray<ObjectType, SizeType>::~CE_GrowingArray()
{
	delete[] myData;
}

template<typename ObjectType, typename SizeType = int>
inline CE_GrowingArray<ObjectType, SizeType>& CE_GrowingArray<ObjectType, SizeType>::operator=(const CE_GrowingArray& aGrowingArray)
{
	// if aGrowingArray.myMaxSize <= myMaxSize, I could just copy everything, don't need newData
	delete[] myData;
	myMaxSize = aGrowingArray.myMaxSize;
	myCurrentSize = aGrowingArray.myCurrentSize;
	myUseSafeModeFlag = aGrowingArray.myUseSafeModeFlag;

	ObjectType* newData = new ObjectType[aGrowingArray.myMaxSize];


	if (myUseSafeModeFlag == true)
	{
		for (SizeType i = 0; i < myCurrentSize; ++i)
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

template<typename ObjectType, typename SizeType = int>
inline void CE_GrowingArray<ObjectType, SizeType>::Respace(int aNewSize)
{
	CE_ASSERT(aNewSize > 0, locGrowingArray_ErrorStrings[INVALID_SIZE]);
	ObjectType* newData = new ObjectType[aNewSize];
	if (myUseSafeModeFlag == true)
	{
		for (SizeType i = 0; i < myCurrentSize; ++i)
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
	myMaxSize = static_cast<SizeType>(aNewSize);
}

template<typename ObjectType, typename SizeType = int>
inline void CE_GrowingArray<ObjectType, SizeType>::Reserve(SizeType aNrOfItems, bool aUseSafeModeFlag = true)
{
	Respace(aNrOfItems);
	myCurrentSize = aNrOfItems;
}

template<typename ObjectType, typename SizeType = int>
inline ObjectType& CE_GrowingArray<ObjectType, SizeType>::operator[](const SizeType& aIndex)
{
	CE_ASSERT(aIndex >= 0, locGrowingArray_ErrorStrings[LOW_INDEX]);
	CE_ASSERT(aIndex < myCurrentSize, locGrowingArray_ErrorStrings[HIGH_INDEX]);
	return myData[aIndex];
}

template<typename ObjectType, typename SizeType = int>
inline const ObjectType& CE_GrowingArray<ObjectType, SizeType>::operator[](const SizeType& aIndex) const
{
	CE_ASSERT(aIndex >= 0, locGrowingArray_ErrorStrings[LOW_INDEX]);
	CE_ASSERT(aIndex < myCurrentSize, locGrowingArray_ErrorStrings[HIGH_INDEX]);
	return myData[aIndex];
}

template<typename ObjectType, typename SizeType = int>
inline void CE_GrowingArray<ObjectType, SizeType>::Add(const ObjectType& aObject)
{
	if (myCurrentSize >= myMaxSize)
	{
		Respace(myMaxSize * 2);
	}
	myData[myCurrentSize++] = aObject;
}


template<typename ObjectType, typename SizeType = int>
inline ObjectType& CE_GrowingArray<ObjectType, SizeType>::Add()
{
	if (myCurrentSize >= myMaxSize)
	{
		Respace(myMaxSize * 2);
	}

	ObjectType& object = myData[myCurrentSize++];
	return object;
}


template<typename ObjectType, typename SizeType = int>
inline void CE_GrowingArray<ObjectType, SizeType>::AddEmptyObject()
{
	if (myCurrentSize == myMaxSize)
	{
		Respace(myMaxSize * 2);
	}

	myCurrentSize++;
}

template<typename ObjectType, typename SizeType = int>
inline bool CE_GrowingArray<ObjectType, SizeType>::AddUnique(const ObjectType& aObject)
{
	if (Find(aObject) == -1)
	{
		Add(aObject);
		return true;
	}

	return false;
}

template<typename ObjectType, typename SizeType = int>
inline void CE_GrowingArray<ObjectType, SizeType>::Insert(SizeType aIndex, const ObjectType& aObject)
{
	CE_ASSERT(aIndex >= 0, locGrowingArray_ErrorStrings[LOW_INDEX]);
	CE_ASSERT(aIndex < myCurrentSize, locGrowingArray_ErrorStrings[HIGH_INDEX]);

	if (myCurrentSize >= myMaxSize)
	{
		Respace(myMaxSize * 2);
	}
	for (SizeType i = myCurrentSize - 1; i >= aIndex; --i)
	{
		if (i == SizeType(-1))
		{
			break;
		}
		myData[i + 1] = myData[i];
	}
	myData[aIndex] = aObject;
	++myCurrentSize;
}

template<typename ObjectType, typename SizeType = int>
inline void CE_GrowingArray<ObjectType, SizeType>::InsertFirst(const ObjectType& aObject)
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

template<typename ObjectType, typename SizeType = int>
inline void CE_GrowingArray<ObjectType, SizeType>::DeleteCyclic(ObjectType& aObject)
{
	for (SizeType i = 0; i < myCurrentSize; ++i)
	{
		if (myData[i] == aObject)
		{
			DeleteCyclicAtIndex(i);
			return;
		}
	}
	CE_ASSERT_ALWAYS("Object not found.");
}

template<typename ObjectType, typename SizeType = int>
inline void CE_GrowingArray<ObjectType, SizeType>::DeleteCyclicAtIndex(SizeType aItemNumber)
{
	CE_ASSERT(aItemNumber >= 0, locGrowingArray_ErrorStrings[LOW_INDEX]);
	CE_ASSERT(aItemNumber < myCurrentSize, locGrowingArray_ErrorStrings[HIGH_INDEX]);

	delete myData[aItemNumber];
	myData[aItemNumber] = nullptr;
	myData[aItemNumber] = myData[--myCurrentSize];
}

template<typename ObjectType, typename SizeType = int>
inline void CE_GrowingArray<ObjectType, SizeType>::DeleteNonCyclicAtIndex(SizeType aItemNumber)
{
	CE_ASSERT(aItemNumber >= 0, locGrowingArray_ErrorStrings[LOW_INDEX]);
	CE_ASSERT(aItemNumber < myCurrentSize, locGrowingArray_ErrorStrings[HIGH_INDEX]);

	delete myData[aItemNumber];
	myData[aItemNumber] = nullptr;

	for (SizeType i = aItemNumber; i < myCurrentSize - 1; ++i)
	{
		myData[i] = myData[i + 1];
	}
	--myCurrentSize;
}


template<typename ObjectType, typename SizeType = int>
inline void CE_GrowingArray<ObjectType, SizeType>::RemoveCyclic(const ObjectType& aObject)
{
	for (SizeType i = 0; i < myCurrentSize; ++i)
	{
		if (myData[i] == aObject)
		{
			RemoveCyclicAtIndex(i);
			return;
		}
	}
	CE_ASSERT_ALWAYS("Object not found.");
}

template<typename ObjectType, typename SizeType = int>
inline void CE_GrowingArray<ObjectType, SizeType>::RemoveCyclicAtIndex(SizeType aItemNumber)
{
	CE_ASSERT(aItemNumber >= 0, locGrowingArray_ErrorStrings[LOW_INDEX]);
	CE_ASSERT(aItemNumber < myCurrentSize, locGrowingArray_ErrorStrings[HIGH_INDEX]);

	myData[aItemNumber] = myData[--myCurrentSize];
}

template<typename ObjectType, typename SizeType = int>
inline void CE_GrowingArray<ObjectType, SizeType>::RemoveNonCyclic(const ObjectType& aObject)
{
	for (SizeType i = 0; i < myCurrentSize; ++i)
	{
		if (myData[i] == aObject)
		{
			RemoveNonCyclicAtIndex(i);
			return;
		}
	}
	CE_ASSERT_ALWAYS("Object not found.");
}


template<typename ObjectType, typename SizeType = int>
inline void CE_GrowingArray<ObjectType, SizeType>::RemoveNonCyclicAtIndex(SizeType aItemNumber)
{
	CE_ASSERT(aItemNumber >= 0, locGrowingArray_ErrorStrings[LOW_INDEX]);
	CE_ASSERT(aItemNumber < myCurrentSize, locGrowingArray_ErrorStrings[HIGH_INDEX]);

	for (SizeType i = aItemNumber; i < myCurrentSize - 1; ++i)
	{
		myData[i] = myData[i + 1];
	}
	--myCurrentSize;
}

template<typename ObjectType, typename SizeType = int>
inline SizeType CE_GrowingArray<ObjectType, SizeType>::Find(const ObjectType& aObject) const
{
	for (SizeType i = 0; i < myCurrentSize; ++i)
	{
		if (myData[i] == aObject)
		{
			return i;
		}
	}
	return FoundNone;
}

template<typename ObjectType, typename SizeType = int>
inline ObjectType& CE_GrowingArray<ObjectType, SizeType>::GetLast()
{
	return myData[myCurrentSize - 1];
}

template<typename ObjectType, typename SizeType = int>
inline const ObjectType& CE_GrowingArray<ObjectType, SizeType>::GetLast() const
{
	return myData[myCurrentSize - 1];
}

template<typename ObjectType, typename SizeType = int>
inline void CE_GrowingArray<ObjectType, SizeType>::RemoveAll()
{
	myCurrentSize = 0;
}

template<typename ObjectType, typename SizeType = int>
inline void CE_GrowingArray<ObjectType, SizeType>::DeleteAll()
{
	for (SizeType i = 0; i < myCurrentSize; ++i)
	{
		delete myData[i];
		myData[i] = nullptr;
	}
	myCurrentSize = 0;
}

template<typename ObjectType, typename SizeType = int>
inline void CE_GrowingArray<ObjectType, SizeType>::Optimize()
{
	myMaxSize = myCurrentSize;
	if (myMaxSize < 1)
	{
		myMaxSize = 1;
	}
	Respace(myMaxSize);
}

template<typename ObjectType, typename SizeType = int>
__forceinline SizeType CE_GrowingArray<ObjectType, SizeType>::Size() const
{
	return myCurrentSize;
}

template<typename ObjectType, typename SizeType = int>
__forceinline SizeType CE_GrowingArray<ObjectType, SizeType>::GetCapacity() const
{
	return myMaxSize;
}

template<typename ObjectType, typename SizeType = int>
inline ObjectType* CE_GrowingArray<ObjectType, SizeType>::GetArrayAsPointer()
{
	return myData;
}