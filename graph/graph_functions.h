#pragma once
#include "graph_utils.h"
#include "undirected_graph.h"
#include "undirected_weighted_graph.h"
namespace graph
{
	template<typename weight_type,typename int_type,typename graph_type>
	graph_type prim(const graph_type &H ,int_type i = 0)
	{
		auto n(H.count_vertices());
		graph_type G(n);
		using w_edge = weighted_edge<weight_type, int_type>;
		std::priority_queue <w_edge, std::vector<w_edge>, std::greater<void>> Q;
		std::vector<weight_type> distances(n, inf<weight_type>);
		Q.push({ i,i,0 });
		while (!Q.empty())
		{
			auto p = Q.top();
			Q.pop();
			if (distances[p.b] < p.w)
				continue;
			distances[p.b] = p.w;
			G.connect(p.a, p.b, p.w);
			for (const auto& e : H.adjacent_list(p.b))
			{
				if (p.w + e.second < distances[e.first])
				{
					Q.push({ p.b,e.first,p.w + e.second });
					distances[e.first] = p.w + e.second;
				}
			}
		}
		G.disconnect(i, i);
		return G;
	}

	template<typename weight_type, typename int_type, typename graph_type>
	graph_type kruskal(const graph_type &H)
	{
		auto n(H.count_vertices());
		graph_type G(n);
		std::set<weighted_edge<weight_type, int_type>> E;
		for (int i = 0; i < n; i++)
			for (const auto& e : H.adjacent_list(i))
				E.insert({ i,e.first,e.second });
		disjoint_set<int_type> S(n);
		for (auto it = E.begin(); it != E.end(); ++it)
		{
			if (!S.connected(it->a, it->b))
			{
				S.set_union(it->a, it->b);
				G.connect(it->a, it->b, it->w);
			}
		}
		return G;
	}
}