#pragma once
#include <limits>
#include <compare>
#include <set>
#include <map>
#include <vector>
namespace graph
{
	template<typename weight_type = double>
	constexpr weight_type inf = std::numeric_limits<weight_type>::infinity();

	template<typename int_type = int>
	int_type int_inf = std::numeric_limits<int_type>::max();

	template<typename weight_type = double,typename int_type=int>
	struct path
	{
		weight_type d;
		int_type p;
	};

	template<typename weight_type = double, typename int_type = int>
	struct weighted_edge
	{
		int_type a, b;
		weight_type w;
		std::partial_ordering operator<=>(const weighted_edge& that) const
		{
			return w <=> that.w;
		}
	};

	template<typename T1, typename T2 = T1>
	struct ordered_couple
	{
		T1 x;
		T2 y;
		std::partial_ordering operator<=>(const ordered_couple& that) const
		{
			if (auto cmp = x <=> that.x; cmp != 0)
				return cmp;
			return y <=> that.y;
		}
	};


	template<typename int_type=int>
	class disjoint_set
	{
		std::vector<int_type> S;
		std::vector<int_type> R;
	public:
		disjoint_set(int_type n) :S(n), R(n)
		{
			for (auto i = 0; i < n; i++)
				S[i] = i;

		}
		void set_union(int_type a, int_type b)
		{
			auto i(find(a)), j(find(b));
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

		bool connected(int_type a, int_type b)
		{
			return find(a) == find(b);
		}

		int_type find(int_type i)
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
}