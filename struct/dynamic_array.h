#pragma once
#include <algorithm>

template<typename T>
class dynamic_array
{
	T* arr;
	// C is array capacity
	// n is array size
	int C, n;
public:

	dynamic_array(int s)
	{
		n = s;
		C = std::max(s,1);
		arr = new T[C];
	}
	dynamic_array():dynamic_array(0)
	{
	}
	dynamic_array(const dynamic_array& O):arr(O.n)
	{
		for (int i = 0; i < n; i++)
			arr[i] = O.arr[i];
	}

	dynamic_array& operator=(const dynamic_array& O)
	{
		ren(O.n);
		for (int i = 0; i < n; i++)
			arr[i] = O.arr[i];
		return *this;
	}

	dynamic_array(dynamic_array&& O) :arr(std::move(O.arr)), C(O.C), n(O.n)
	{
		O.arr = nullptr;
		O.n = 0;
		O.C = 0;
	}

	~dynamic_array()
	{
		delete[] arr;
	}

	void push_back(T u)
	{
		if (n == C)
			set_capacity(2 * C);
		arr[n] = u;
		n++;
	}

	void pop_back()
	{
		if (n < C / 4)
			set_capacity(C / 4);
		n--;
	}

	void ren(int s)
	{
		set_capacity(s);
		n = s;
	}

	void set_capacity(int _C)
	{
		C = _C;
		auto P = new T[C];
		for (int i = 0; i < std::min(n,C); i++)
			P[i] = arr[i];
		delete[] arr;
		arr = P;
	}

	T& operator[](int i)
	{
		return arr[i];
	}

	const T& operator[](int i) const
	{
		return arr[i];
	}

	T front() const
	{
		return arr[0];
	}

	T back() const
	{
		return arr[n - 1];
	}

	int size() const
	{
		return n;
	}

	int capacity() const
	{
		return C;
	}
};

