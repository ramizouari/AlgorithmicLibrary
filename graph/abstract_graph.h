#pragma once
#include <vector>
#include <set>
#include <queue>
#include "graph_utils.h"

namespace graph {
	template<typename int_type = int, typename container = std::set<int_type>>
	class abstract_graph
	{
	public:
		abstract_graph(int_type n) :adjacency_list(n)
		{}
		int_type count_vertices() const { return adjacency_list.size(); }
		int_type count_edges() const
		{
			int_type E(0);
			for (const auto& list : adjacency_list)
				E += list.size();
			return E;
		}
		virtual int_type distance(int_type i, int_type j) const
		{
			auto n(count_vertices());
			std::vector<bool> visited(n, false);
			std::vector<int_type> distances(n);
			std::queue<int_type> Q;
			Q.push(i);
			visited[i] = true;
			distances[i] = 0;
			while (!Q.empty())
			{
				auto w = Q.front();
				Q.pop();
				for (const auto& p : adjacency_list[w])
				{
					if (visited[p])
						continue;
					visited[p] = true;
					distances[p] = distances[w] + 1;
					Q.push(p);
					if (p == j)
						return distances[p];
				}
			}
			return int_inf;
		}
		virtual std::vector<int_type> shortest_path(int_type i, int_type j) const
		{
			auto n(count_vertices());
			std::vector<bool> visited(n, false);
			std::vector<path<int_type>> P(n);
			std::queue<int_type> Q;
			Q.push(i);
			visited[i] = true;
			P[i] = { 0,i };
			while (!Q.empty())
			{
				auto w = Q.front();
				Q.pop();
				for (const auto& p : adjacency_list[w])
				{
					if (visited[p])
						continue;
					visited[p] = true;
					P[p] = { P[w].d + 1, w };
					Q.push(p);
					if (p == j)
						break;
				}
			}
			std::vector<int_type> result;
			auto p = j;
			while (P[j].p != j)
			{
				result.push_back(j);
				j = P[j].p;
			}
			result.push_back(j);
			std::reverse(result.begin(), result.end());
			return result;
		}

		virtual void connect(int_type i, int_type j) = 0;
		virtual void disconnect(int_type i, int_type j) = 0;

	protected:
		std::vector<container> adjacency_list;
	};
}