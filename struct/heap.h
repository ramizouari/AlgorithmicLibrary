#pragma once
#include "dynamic_array.h"
#include "stack.h"
// To do: heap using general total order relation

namespace heap_utils
{
	template<typename T>
	void lift_up(dynamic_array<T>& S, int s)
	{
		while (s > 0)
		{
			if (S[s] > S[(s - 1) / 2])
			{
				std::swap(S[s], S[(s - 1) / 2]);
				s = (s - 1) / 2;
			}
			else break;
		}
	}

	template<typename T>
	void lift_down(dynamic_array<T>& S, int s, int size)
	{
		int p(s);
		while ((2 * s + 1) < size)
		{
			p = 2 * s + 1;
			if (((2 * s + 2) < size) && (S[2 * s + 2] > S[2 * s + 1]))
				p = 2 * s + 2;

			if (S[s] < S[p])
			{
				std::swap(S[s], S[p]);
				s = p;
			}
			else break;
		}
	}

	stack<bool> trace_trajectory(int s)
	{
		stack<bool> M;
		while (s > 0)
		{
			M.push(s % 2);
			s = (s - 1) / 2;
		}
		return M;
	}

	template<typename T>
	T pop_max(dynamic_array<T>& S, int& size)
	{
		T w = S[0];
		int p(0);
		stack<bool> traj = trace_trajectory(size - 1);
		while (!traj.empty())
			if (traj.pop())
				p = 2 * p + 1;
			else p = 2 * p + 2;
		std::swap(S[0], S[p]);
		size--;
		lift_down(S, 0, size);
		return w;
	}

	template<typename T>
	void heap_sort(dynamic_array<T>& arr)
	{
		int n = arr.size();
		int size = n;
		for (int i = n / 2 + 1; i >= 0; i--)
			lift_down(arr, i, size);
		for (int i = 0; i < n; i++)
			pop_max(arr, size);

	}

	template<typename T>
	bool is_heap(const dynamic_array<T> &S)
	{
		int size = S.size();
		for (int i = 0; i < size / 2; i++)
		{
			if ((2 * i + 1 < size) && (S[i] < S[2 * i + 1]))
				return false;
			if ((2 * i + 2 < size) && (S[i] < S[2 * i + 2]))
				return false;
		}
		return true;
	}
}

using heap_utils::heap_sort;

template<typename T>
class heap
{
	dynamic_array<T> S;
	int size;
public:
	heap():size(0){}

	void insert(T w)
	{
		if (size == S.get_capacity())
			S.resize(2 * size);
		int p(0);
		int s = size;
		stack<bool> traj = heap_utils::trace_trajectory(size);
		while (!traj.empty())
			if (traj.pop())
				p = 2 * p + 1;
			else p = 2 * p + 2;
		S[p] = w;
		size++;
		heap_utils::lift_up(S,size-1);
	}

	T get_max() const
	{
		return S[0];
	}

	T pop()
	{
		return heap_utils::pop_max(S, size);
	}

	bool empty() const
	{
		return size == 0;
	}

private:
	
public:
	
};