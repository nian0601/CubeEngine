#pragma once

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