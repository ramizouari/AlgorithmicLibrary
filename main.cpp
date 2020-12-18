#include "graph/undirected_weighted_graph.h"
#include "graph/directed_weighted_graph.h"
#include "graph/graph_functions.h"
#include <iostream>

using namespace std;

int main()
{
	int n, m;
	cin >> n >> m;
	int a, b;
	using namespace graph;
	undirected_weighted_graph G(n);
	double w;
	for (int i = 0; i < m; i++)
	{
		cin >> a >> b >> w;
		G.connect(a, b,w);
	}
	cin >> a >> b;
	for (auto& e : G.shortest_path(a, b))
		cout << e << '\t';
	cout << endl << G.distance(a, b) << endl;
	for (auto& e : G.bellman_ford_shortest_path(a, b))
		cout << e << '\t';
	auto G1(graph::kruskal<double,int, undirected_weighted_graph>(G)), G2(G.prim());
	return false;
}