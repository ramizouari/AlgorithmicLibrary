#pragma once
#include "dynamic_array.h"


class disjoint_set
{
	dynamic_array<int> S;
	dynamic_array<int> R;
public:
	disjoint_set(int n) :S(n),R(n)
	{
		for (int i = 0; i < n; i++)
			S[i] = i;
		
	}
	void set_union(int a, int b)
	{
		int i(find(a)), j(find(b));
		if (a == b)
			return;
		if (R[i] > R[j])
			S[j] = S[i];
		else if (R[i] < R[j])
			S[i] = S[j];
		else
		{
			S[j] = S[i];
			R[i]++;
		}
			
		
	}

	bool connected(int a, int b)
	{
		return find(a) == find(b);
	}

	int find(int i)
	{
		if (i != S[i])
			S[i] = find(S[i]);
		
		return S[i];
	}

	void add()
	{
		S.push_back(S.size());
		R.push_back(0);
	}
};