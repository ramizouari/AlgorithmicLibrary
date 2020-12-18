#pragma once
#include "abstract_graph.h"

namespace graph {
	template<typename int_type = int, typename container = std::set<int_type>>
	class basic_directed_graph : public abstract_graph<int_type, container>
	{
	public:
		basic_directed_graph(int_type n) :abstract_graph<int_type, container>(n) {}
		void connect(int_type i, int_type j)
		{
			this->adjacency_list[i].insert(j);
		}
		void disconnect(int_type i, int_type j)
		{
			this->adjacency_list[i].erase(j);
		}
		basic_directed_graph reverse() const
		{
			auto n(count_vertices());
			basic_directed_graph R(n);
			for (int i = 0; i < n; i++)
				for (const auto& p : adjacency_list[i])
					R.connect(p, i);
			return R;
		}
	};

	using directed_graph = basic_directed_graph<>;
}