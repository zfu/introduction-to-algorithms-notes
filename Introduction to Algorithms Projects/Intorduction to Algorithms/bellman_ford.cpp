#include "stdafx.h"

#include <vector>
#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <iterator>
#include <bitset>
#include <queue>
#include <limits>
#include "graphics.h"

using namespace std;
using namespace chapter22;

namespace chapter24
{

	void InitializeSingleSource(GraphicsViaAdjacencyList<char> &g, vector<int> &d, vector<int> &parent_index, int start_index)
	{
		for (size_t i = 0; i < g.GetVertex().size(); ++i)
		{
			d[i] = numeric_limits<int>::max();
			parent_index[i] = -1;
		}
		d[start_index] = 0;
	}

	/// 松弛
	/// 
	/// 前提条件是：index1->index2存在着一条边
	/// 因此用index1的这条边来松弛index2的d值，并且设置parent[index2] = index1
	void Relax(GraphicsViaAdjacencyList<char> &g, vector<int> &d, vector<int> &parent_index, int index1, int index2)
	{
		if (d[index2] > d[index1] + g.IsLinked(index1, index2).second)
		{
			d[index2] = d[index1] + g.IsLinked(index1, index2).second;
			parent_index[index2] = index1;
		}
	}


	bool BellmanFord()
	{
		cout << "Bellman-Ford最短路径" << endl;
		//生成P362页的图24-4
		vector<char> v;
		v.push_back('s');
		v.push_back('t');
		v.push_back('x');
		v.push_back('z');
		v.push_back('y');
		GraphicsViaAdjacencyList<char> g(v, Digraph);
		g.Link2Vertex(0, 1, 6);
		g.Link2Vertex(0, 4, 7);
		g.Link2Vertex(1, 2, 5);
		g.Link2Vertex(1, 3, -4);
		g.Link2Vertex(1, 4, 8);
		g.Link2Vertex(2, 1, -2);
		g.Link2Vertex(3, 2, 7);
		g.Link2Vertex(3, 0, 2);
		g.Link2Vertex(4, 2, -3);
		g.Link2Vertex(4, 3, 9);

		int start_index = 0;
		vector<int> d(g.GetVertex().size());
		vector<int> parent_index(g.GetVertex().size());
		auto edges = g.GetAllEdges();

		InitializeSingleSource(g, d, parent_index, start_index);		

		for (int i = 0; i < g.GetVertex().size() - 1; ++i)
		{			
			for_each(edges.begin(), edges.end(), [&](pair<size_t, size_t> const &p){
				Relax(g, d, parent_index, p.first, p.second);
			});
		}

		for (int i = 0; i < edges.size(); ++i)
		{
			if (d[edges[i].second] > d[edges[i].first] + g.IsLinked(edges[i].first, edges[i].second).second)
			{
				cout << "存在负值回路" << endl;
				return false;
			}
		}

		for (size_t i = 0; i < g.GetVertex().size(); ++i)
		{
			cout << g.GetVertex()[i] << " | " << d[i] << endl;
		}
		return true;
	}
}