#pragma once

#include "CE_Hashing.h"
#include "CE_GrowingArray.h"

template<typename Key, typename Value>
struct CE_MapIterator
{
	CE_MapIterator(int aFirst, int aSecond, Key* aKey, Value* aValue)
		: myFirstIndex(aFirst)
		, mySecondIndex(aSecond)
		, myKey(aKey)
		, myValue(aValue)
	{}

	bool operator==(const CE_MapIterator &aIterator) const;
	bool operator!=(const CE_MapIterator &aIterator) const;

	Key& First() { return *myKey; }
	Value& Second() { return *myValue; }

	int myFirstIndex;
	int mySecondIndex;

private:
	Key* myKey;
	Value* myValue;
};

template<typename Key, typename Value>
bool CE_MapIterator<Key, Value>::operator==(const CE_MapIterator &aIterator) const
{
	return myFirstIndex == aIterator.myFirstIndex && mySecondIndex == aIterator.mySecondIndex;
}

template<typename Key, typename Value>
bool CE_MapIterator<Key, Value>::operator!=(const CE_MapIterator &aIterator) const
{
	return (*this == aIterator) == false;
}

template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
class CE_Map
{
public:
	CE_Map();
	~CE_Map();

	void Insert(const Key &aKey, const Value &aValue);
	Value& Get(const Key &aKey);
	const Value& Get(const Key &aKey) const;
	const Value* GetIfExists(const Key &aKey) const;
	void Remove(const Key &aKey);
	bool KeyExists(const Key &aKey) const;
	Value& operator[](const Key &aKey);

	CE_MapIterator<Key, Value> Begin();
	CE_MapIterator<Key, Value> Next(CE_MapIterator<Key, Value>& aCurrent);
	CE_MapIterator<Key, Value> End();

	void Clear();

private:
	struct KeyValuePair
	{
		Key myKey;
		Value myValue;
	};

	int FindKeyInBucket(int aBucketIndex, const Key& aKey) const;

	int OwnHash(const Key &aKey) const;
	CE_GrowingArray<CE_GrowingArray<KeyValuePair>> myBuckets;
};


//----- CONSTRUCTOR -----
//------------------------
template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
CE_Map<Key, Value, StartSize, BucketSize>::CE_Map()
{
	myBuckets.Reserve(StartSize);

	for (int i = 0; i < myBuckets.Size(); ++i)
	{
		myBuckets[i].Respace(BucketSize);
	}
}


//----- DECONSTRUCTOR ----
//------------------------
template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
CE_Map<Key, Value, StartSize, BucketSize>::~CE_Map()
{

}


//----- INSERT -----
//------------------
template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
void CE_Map<Key, Value, StartSize, BucketSize>::Insert(const Key &aKey, const Value &aValue)
{
	if (KeyExists(aKey) == true)
	{
		operator[](aKey) = aValue;
		return;
	}

	int index = OwnHash(aKey);

	KeyValuePair pair;
	pair.myKey = aKey;
	pair.myValue = aValue;

	myBuckets[index].Add(pair);
}


//----- GET -----
//---------------
template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
Value& CE_Map<Key, Value, StartSize, BucketSize>::Get(const Key &aKey)
{
	int index = OwnHash(aKey);
	int keyIndex = FindKeyInBucket(index, aKey);
	CE_ASSERT(keyIndex != -1, "[CE_Map]: Tried to get an nonexisting Key.");

	return myBuckets[index][keyIndex].myValue;
}

template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
const Value& CE_Map<Key, Value, StartSize, BucketSize>::Get(const Key &aKey) const
{
	int index = OwnHash(aKey);
	int keyIndex = FindKeyInBucket(index, aKey);
	CE_ASSERT(keyIndex != -1, "[CE_Map]: Tried to get an nonexisting Key.");

	return myBuckets[index][keyIndex].myValue;
}

template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
const Value* CE_Map<Key, Value, StartSize, BucketSize>::GetIfExists(const Key &aKey) const
{
	if (!KeyExists(aKey))
		return nullptr;

	return &Get(aKey);
}

//----- DELETE -----
//------------------

template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
void CE_Map<Key, Value, StartSize, BucketSize>::Remove(const Key &aKey)
{
	CE_ASSERT(KeyExists(aKey) == true, "[CE_Map]: Tried to delete an nonexisting Key.");

	int index = OwnHash(aKey);

	for (int i = 0; i < myBuckets[index].Size(); ++i)
	{
		if (myBuckets[index][i].myKey == aKey)
		{
			myBuckets[index].RemoveCyclicAtIndex(i);
			return;
		}
	}
}


//----- KEY_EXISTS -----
//---------------------
template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
bool CE_Map<Key, Value, StartSize, BucketSize>::KeyExists(const Key &aKey) const
{
	int index = OwnHash(aKey);
	return FindKeyInBucket(index, aKey) != -1;
}


//----- SUBSCRIPT_OPERATOR -----
//------------------------------
template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
Value& CE_Map<Key, Value, StartSize, BucketSize>::operator[](const Key &aKey)
{
	int index = OwnHash(aKey);
	if (FindKeyInBucket(index, aKey) == -1)
	{
		KeyValuePair pair;
		pair.myKey = aKey;
		pair.myValue = Value();

		CE_GrowingArray<KeyValuePair>& bucket = myBuckets[index];
		bucket.Add(pair);
		return bucket.GetLast().myValue;
	}

	return Get(aKey);
}

template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
CE_MapIterator<Key, Value> CE_Map<Key, Value, StartSize, BucketSize>::Begin()
{
	int first = -1;
	int second = -1;
	Key* key = nullptr;
	Value* value = nullptr;

	for (int i = 0; i < myBuckets.Size(); ++i)
	{
		if (myBuckets[i].Size() > 0)
		{
			first = i;
			second = 0;

			key = &myBuckets[first][second].myKey;
			value = &myBuckets[first][second].myValue;
			break;
		}
	}

	return CE_MapIterator<Key, Value>(first, second, key, value);
}

template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
CE_MapIterator<Key, Value> CE_Map<Key, Value, StartSize, BucketSize>::Next(CE_MapIterator<Key, Value>& aCurrent)
{
	int first = -1;
	int second = -1;
	Key* key = nullptr;
	Value* value = nullptr;

	int innerIndex = aCurrent.mySecondIndex + 1;

	for (int i = aCurrent.myFirstIndex; i < myBuckets.Size(); ++i)
	{
		CE_GrowingArray<KeyValuePair>& bucket = myBuckets[i];
		for (int j = innerIndex; j < bucket.Size(); ++j)
		{
			first = i;
			second = j;

			key = &myBuckets[first][second].myKey;
			value = &myBuckets[first][second].myValue;
			break;
		}

		innerIndex = 0;
		if (key != nullptr)
		{
			break;
		}

	}

	return CE_MapIterator<Key, Value>(first, second, key, value);
}

template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
CE_MapIterator<Key, Value> CE_Map<Key, Value, StartSize, BucketSize>::End()
{
	return CE_MapIterator<Key, Value>(-1, -1, nullptr, nullptr);
}

template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
void CE_Map<Key, Value, StartSize, BucketSize>::Clear()
{
	for (int i = 0; i < myBuckets.Size(); ++i)
	{
		myBuckets[i].RemoveAll();
	}
}


template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
int CE_Map<Key, Value, StartSize, BucketSize>::FindKeyInBucket(int aBucketIndex, const Key& aKey) const
{
	const CE_GrowingArray<KeyValuePair>& bucket = myBuckets[aBucketIndex];

	for (int i = 0; i < bucket.Size(); ++i)
	{
		if (bucket[i].myKey == aKey)
			return i;
	}

	return -1;
}

//----- HASH -----
//----------------
template<typename Key, typename Value, int StartSize = 67, int BucketSize = 3>
int CE_Map<Key, Value, StartSize, BucketSize>::OwnHash(const Key &aKey) const
{
	return Murmur(aKey) % StartSize;
}