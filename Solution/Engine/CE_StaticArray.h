#pragma once

template<typename Type, int Size>
class CE_StaticArray
{
public:
	CE_StaticArray();
	CE_StaticArray(const CE_StaticArray& aStaticArray);

	~CE_StaticArray();

	CE_StaticArray& operator=(const CE_StaticArray& aStaticArray);

	inline const Type& operator[](const int& aIndex) const;
	inline Type& operator[](const int& aIndex);
	inline int Count() const { return Size; };

	// Utility functions
	inline void Insert(int aIndex, Type& aObject);
	inline void DeleteAll();

	typedef Type* iterator;
	typedef const Type* const_iterator;
	iterator begin() { return &myData[0]; }
	const_iterator begin() const { return &myData[0]; }
	iterator end() { return &myData[Size]; }
	const_iterator end() const { return &myData[Size]; }
private:
	Type myData[Size];
};

template<typename Type, int Size>
CE_StaticArray<Type, Size>::CE_StaticArray()
{

}

template<typename Type, int Size>
CE_StaticArray<Type, Size>::~CE_StaticArray()
{

}

template<typename Type, int Size>
CE_StaticArray<Type, Size>::CE_StaticArray(const CE_StaticArray& aStaticArray)
{
	operator=(aStaticArray);
}

template<typename Type, int Size>
CE_StaticArray<Type, Size>& CE_StaticArray<Type, Size>::operator=(const CE_StaticArray& aStaticArray)
{
	for (int i = 0; i < Size; ++i)
	{
		myData[i] = aStaticArray.myData[i];
	}
	return *this;
}

template<typename Type, int Size>
inline const Type& CE_StaticArray<Type, Size>::operator[](const int& aIndex) const
{
	CE_ASSERT(aIndex >= 0, "Index has to be 0 or more.");
	CE_ASSERT(aIndex < Size, "a index out of bounds!");
	return myData[aIndex];
}

template<typename Type, int Size>
inline Type& CE_StaticArray<Type, Size>::operator[](const int& aIndex)
{
	CE_ASSERT(aIndex >= 0, "Index has to be 0 or more.");
	CE_ASSERT(aIndex < Size, "a index out of bounds!");
	return myData[aIndex];
}

template<typename Type, int Size>
inline void CE_StaticArray<Type, Size>::Insert(int aIndex, Type& aObject)
{
	CE_ASSERT(aIndex >= 0, "Index has to be 0 or more.");
	CE_ASSERT(aIndex < Size, "a index out of bounds!");
	for (int i = Size - 2; i >= aIndex; --i)
	{
		myData[i + 1] = myData[i];
	}
	myData[aIndex] = aObject;
}

template<typename Type, int Size>
inline void CE_StaticArray<Type, Size>::DeleteAll()
{
	for (int i = 0; i < Size; ++i)
	{
		delete myData[i];
		myData[i] = nullptr;
	}
}