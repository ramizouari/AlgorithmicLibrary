#pragma once
#include "abstract_weighted_graph.h"

namespace graph {
	template<typename weight_type = double, typename int_type = int, typename container = std::map<int_type, weight_type>>
	class basic_directed_weighted_graph : public abstract_weighted_graph<weight_type, int_type, container>
	{
	public:
		basic_directed_weighted_graph(int_type n) :abstract_weighted_graph<weight_type, int_type, container>(n) {}
		void connect(int_type i, int_type j, weight_type w)
		{
			this->adjacency_list[i][j] = w;
		}
		void disconnect(int_type i, int_type j)
		{
			this->adjacency_list[i].erase(j);
		}

		basic_directed_weighted_graph reverse() const
		{
			auto n(this->count_vertices());
			basic_directed_weighted_graph R(n);
			for (int i = 0; i < n; i++)
				for (const auto& p : this->adjacency_list[i])
					R.connect(p, i);
			return R;
		}

	};

	using directed_weighted_graph = basic_directed_weighted_graph<>;
}