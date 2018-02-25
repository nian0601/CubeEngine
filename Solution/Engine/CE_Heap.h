#pragma once
#include "CE_GrowingArray.h"

template<typename T, typename Comparer>
class CE_Heap
{
public:
	CE_Heap() {}
	CE_Heap(CE_GrowingArray<T> aGrowingArray);

	void Enqueue(const T& someData);
	T Dequeue();
	T Peak();

	bool IsEmpty() const;

	void Heapify();
	void Clear();

private:
	void MoveDown(int aFirst, int aLast);
	void Swap(T& a, T& b);
	bool Compare(int aFirst, int aSecond);

	CE_GrowingArray<T> myData;
	Comparer myComparer;
};

template<typename T, typename Comparer>
CE_Heap<T, Comparer>::CE_Heap(CE_GrowingArray<T> aGrowingArray)
	: myData(aGrowingArray)
{
	Heapify();
}


template<typename T, typename Comparer>
void CE_Heap<T, Comparer>::Enqueue(const T& someData)
{
	myData.Add(someData);
	int pos = myData.Size() - 1;
	while (pos > 0 && Compare(pos, (pos - 1) / 2))
	{
		Swap(myData[(pos - 1) / 2], myData[pos]);
		pos = (pos - 1) / 2;
	}
}

template<typename T, typename Comparer>
T CE_Heap<T, Comparer>::Dequeue()
{
	T result = myData[0];
	myData[0] = myData[myData.Size() - 1];
	myData.RemoveCyclicAtIndex(myData.Size() - 1);
	int pos = 0;
	int lastNonLeaf = myData.Size() / 2 - 1;
	while (pos < lastNonLeaf
		&& (Compare(pos * 2 + 1, pos) || Compare(pos * 2 + 2, pos)))
	{
		if (Compare(pos * 2 + 1, pos * 2 + 2))
		{
			Swap(myData[pos], myData[pos * 2 + 1]);
			pos = pos * 2 + 1;
		}
		else
		{
			Swap(myData[pos], myData[pos * 2 + 2]);
			pos = pos * 2 + 2;
		}
	}

	if (pos * 2 + 2 < myData.Size() && Compare(pos * 2 + 2, pos))
	{
		Swap(myData[pos], myData[pos * 2 + 2]);
	}

	if (pos * 2 + 1 < myData.Size() && Compare(pos * 2 + 1, pos))
	{
		Swap(myData[pos], myData[pos * 2 + 1]);
	}

	return result;
}

template<typename T, typename Comparer>
T CE_Heap<T, Comparer>::Peak()
{
	return myData[0];
}

template<typename T, typename Comparer>
bool CE_Heap<T, Comparer>::IsEmpty() const
{
	return myData.Size() == 0;
}

template<typename T, typename Comparer>
void CE_Heap<T, Comparer>::Heapify()
{
	int lastNonLeaf = myData.Size() / 2 - 1;
	for (int i = lastNonLeaf; i >= 0; --i)
	{
		MoveDown(i, myData.Size() - 1);
	}
}

template<typename T, typename Comparer>
void CE_Heap<T, Comparer>::Clear()
{
	myData.RemoveAll();
}

template<typename T, typename Comparer>
void CE_Heap<T, Comparer>::MoveDown(int aFirst, int aLast)
{
	int first = aFirst;
	int largest = 2 * first + 1;

	while (largest <= aLast)
	{
		if (largest < aLast && Compare(largest + 1, largest))
		{
			++largest;
		}

		if (Compare(largest, first))
		{
			Swap(myData[largest], myData[first]);
			first = largest;
			largest = 2 * first + 1;
		}
		else
		{
			largest = aLast + 1;
		}
	}
}

template<typename T, typename Comparer>
void CE_Heap<T, Comparer>::Swap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}

template<typename T, typename Comparer>
bool CE_Heap<T, Comparer>::Compare(int aFirst, int aSecond)
{
	return myComparer(myData[aFirst], myData[aSecond]);
}

template<typename T>
struct CE_Comparer
{
	bool operator()(const T a, const T b)
	{
		return a < b;
	}
};

template<typename T>
struct CE_PointerComparer
{
	bool operator()(const T a, const T b)
	{
		return *a < *b;
	}
};