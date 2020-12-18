#pragma once
#include "abstract_weighted_graph.h"
#include "graph_functions.h"

namespace graph {
	template<typename weight_type = double, typename int_type = int, typename container = std::map<int_type, weight_type>>
	class basic_undirected_weighted_graph : public abstract_weighted_graph<weight_type, int_type, container>
	{
	public:
		basic_undirected_weighted_graph(int_type n) :abstract_weighted_graph<weight_type,int_type, container>(n) {}
		void connect(int_type i, int_type j,weight_type w)
		{
			this->adjacency_list[i][j] = w;
			this->adjacency_list[j][i] = w;
		}
		void disconnect(int_type i, int_type j)
		{
			this->adjacency_list[i].erase(j);
			this->adjacency_list[j].erase(i);
		}

		basic_undirected_weighted_graph kruskal() const
		{
			return graph::kruskal<weight_type, int_type, basic_undirected_weighted_graph>(*this);
		}


		basic_undirected_weighted_graph prim(int i=0) const
		{
			return graph::prim<weight_type,int_type,basic_undirected_weighted_graph>(*this, i);
		}
	};

	using undirected_weighted_graph = basic_undirected_weighted_graph<>;
}
