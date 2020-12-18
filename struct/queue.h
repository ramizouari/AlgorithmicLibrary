#pragma once
#include "list.h"

template<typename T>
class queue
{
	list<T> S;
public:
	void push(T w)
	{
		S.push_back(w);
	}

	T pop()
	{
		T p = S.front();
		S.pop();
		return p;
	}

	bool empty() const
	{
		return S.get_length() == 0;
	}

	int size() const
	{
		return S.get_length();
	}
};