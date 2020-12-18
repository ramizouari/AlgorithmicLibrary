#pragma once
#include "abstract_graph.h"

namespace graph {
	template<typename int_type = int, typename container = std::set<int_type>>
	class basic_undirected_graph : public abstract_graph<int_type, container>
	{
	public:
		basic_undirected_graph(int_type n) :abstract_graph<int_type, container>(n) {}
		void connect(int_type i, int_type j)
		{
			this->adjacency_list[i].insert(j);
			this->adjacency_list[j].insert(i);
		}
		void disconnect(int_type i, int_type j)
		{
			this->adjacency_list[i].erase(j);
			this->adjacency_list[j].erase(i);
		}
		
	};

	using undirected_graph = basic_undirected_graph<>;
}