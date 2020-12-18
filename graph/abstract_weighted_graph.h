#pragma once
#include <vector>
#include <map>
#include <queue>
#include <compare>
#include "graph_utils.h"


namespace graph
{
	template<typename weighted_graph_type>
	weighted_graph_type reverse(const weighted_graph_type& G)
	{
		auto n(G.count_vertices());
		weighted_graph_type H(n);
		for (int i = 0; i < n; i++)
			for (const auto& e : G.adjacent_list(i))
				H.connect(e.first, i, e.second);
		return H;

	}


	template<typename weight_type, typename int_type = int,
		typename container = std::map<int_type, weight_type>>
		class abstract_weighted_graph
	{
	public:
		abstract_weighted_graph(int_type n) :adjacency_list(n)
		{}
		int_type count_vertices() const { return adjacency_list.size(); }
		int_type count_edges() const
		{
			int_type E(0);
			for (const auto& list : adjacency_list)
				E += list.size();
			return E;
		}

		bool is_connected() const
		{
			auto n(count_vertices());
			std::queue<int_type> Q;
			Q.push(0);
			std::vector<int_type> visited(n, false);
			visited[0] = true;
			while (!Q.empty())
			{
				auto w = Q.front();
				Q.pop();
				for (const auto& s : adjacency_list[w])
					if (!visited[s])
					{
						visited[s] = true;
						Q.push(s);
					}
			}
			return std::all_of(visited.begin(), visited.end(), [](auto w) {return w; });
		}

		bool is_connected(int_type i, int_type j) const
		{
			auto n(count_vertices());
			std::queue<int_type> Q;
			Q.push(i);
			std::vector<int_type> visited(n, false);
			visited[i] = true;
			while (!Q.empty())
			{
				auto w = Q.front();
				Q.pop();
				for (const auto& s : adjacency_list[w])
					if (!visited[s])
					{
						visited[s] = true;
						Q.push(s);
					}
			}
			return visited[j];
		}

		bool is_acyclic() const
		{

		}

		weight_type dijkstra(int_type i, int_type j) const
		{
			auto n(count_vertices());
			std::vector<bool> visited(n, false);
			std::vector<weight_type> distances(n, inf<weight_type>);
			using couple = ordered_couple<weight_type, int_type>;
			std::priority_queue<couple, std::vector<couple>, std::greater<>> Q;
			Q.push({ 0,i });
			while (!Q.empty())
			{
				auto u = Q.top();
				Q.pop();
				if (u.x > distances[u.y])
					continue;
				distances[u.y] = u.x;
				visited[u.y] = true;
				if (u.y == j)
					break;
				for (const auto& p : adjacency_list[u.y])
				{
					if (distances[u.y] + p.second < distances[p.first])
						Q.push({ distances[u.y] + p.second,p.first});
				}
			}
			return distances[j];
		}

		weight_type bellman_ford(int_type i, int_type j) const
		{
			auto n(count_vertices());
			std::vector<weight_type> distances(n, inf<weight_type>);
			bool updated;
			for (int k = 0; k < n-1; k++)
			{
				updated = false;
				for (int i = 0; i < n; i++)
				{
					for (const auto& e : adjacency_list[i])
					{
						if (distances[i] + e.second < distances[e.first])
						{
							updated = true;
							distances[e.first] = distances[i] + e.second;
						}
					}
				}
				if (!updated)
					break;
			}
			return distances[j];
		}

		std::vector<int_type> bellman_ford_shortest_path(int_type i, int_type j) const
		{
			auto n(count_vertices());
			std::vector<weight_type> distances(n, inf<weight_type>);
			std::vector<path<weight_type, int_type>> P(n);

			bool updated;
			distances[i] = 0;
			P[i].p= i;
			for (int k = 0; k < n - 1; k++)
			{
				updated = false;
				for (int i = 0; i < n; i++)
				{
					for (const auto& e : adjacency_list[i])
					{
						if (distances[i] + e.second < distances[e.first])
						{
							updated = true;
							distances[e.first] = distances[i] + e.second;
							P[e.first].p = i;
						}
					}
				}
				if (!updated)
					break;
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

		virtual weight_type distance(int_type i, int_type j) const
		{
			return bellman_ford(i, j);
		}
		virtual std::vector<int_type> shortest_path(int_type i, int_type j) const
		{
			auto n(count_vertices());
			std::vector<bool> visited(n, false);
			std::vector<weight_type> distances(n, inf<weight_type>);
			std::vector<path<weight_type,int_type>> P(n);
			using couple = ordered_couple<weight_type, int_type>;
			std::priority_queue<couple, std::vector<couple>, std::greater<void>> Q;
			Q.push({ 0,i });
			P[i].p = i;
			while (!Q.empty())
			{
				auto u = Q.top();
				Q.pop();
				if (u.x > distances[u.y])
					continue;
				distances[u.y] = u.x;
				visited[u.y] = true;
				if (u.y == j)
					break;
				for (const auto& p : adjacency_list[u.y])
				{
					if (distances[u.y] + u.x < distances[p.first])
					{
						Q.push({ distances[u.y] + p.second,p.first });
						P[p.first] = { distances[u.y] + p.second,u.y };
					}
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

		virtual void connect(int_type i, int_type j, weight_type w) = 0;
		virtual void disconnect(int_type i, int_type j) = 0;
		const container& adjacent_list(int_type i) const
		{
			return adjacency_list[i];
		}
		protected:
		std::vector<container> adjacency_list;
	};

}