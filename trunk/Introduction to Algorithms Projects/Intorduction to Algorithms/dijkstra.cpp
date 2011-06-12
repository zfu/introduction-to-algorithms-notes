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
	void InitializeSingleSource(GraphicsViaAdjacencyList<char> &g, vector<int> &d, vector<int> &parent_index, int start_index);

	/// 松弛
	/// 
	/// 前提条件是：index1->index2存在着一条边
	/// 因此用index1的这条边来松弛index2的d值，并且设置parent[index2] = index1
	void Relax(GraphicsViaAdjacencyList<char> &g, vector<int> &d, vector<int> &parent_index, int index1, int index2);


	void Dijkstra()
	{
		cout << "Dijkstra最短路径" << endl;
		//生成P367页的图24-6
		vector<char> v;
		v.push_back('s');
		v.push_back('t');
		v.push_back('x');
		v.push_back('z');
		v.push_back('y');
		GraphicsViaAdjacencyList<char> g(v, Digraph);
		g.Link2Vertex(0, 1, 10);
		g.Link2Vertex(0, 4, 5);
		g.Link2Vertex(1, 2, 1);
		g.Link2Vertex(1, 4, 2);
		g.Link2Vertex(2, 3, 4);
		g.Link2Vertex(3, 2, 6);
		g.Link2Vertex(3, 0, 7);
		g.Link2Vertex(4, 1, 3);
		g.Link2Vertex(4, 2, 9);
		g.Link2Vertex(4, 3, 2);

		int start_index = 0;
		vector<int> d(g.GetVertex().size());
		vector<int> parent_index(g.GetVertex().size());
		auto edges = g.GetAllEdges();

		InitializeSingleSource(g, d, parent_index, start_index);

		vector<int> Q;
		for (int i = 0; i < v.size(); ++i)
		{
			Q.push_back(i);
		}

		while(!Q.empty())
		{
			auto min_ele = min_element(Q.begin(), Q.end(), [&](int index1, int index2){
				return d[index1] < d[index2];
			});

			for_each(edges.begin(), edges.end(), [&](pair<size_t, size_t> const &p){
				if (p.first == *min_ele)
				{
					//对所有从min_ele发出的边进行松弛操作
					Relax(g, d, parent_index, p.first, p.second);
				}
			});

			Q.erase(min_ele);
		}
		

		for (size_t i = 0; i < g.GetVertex().size(); ++i)
		{
			cout << g.GetVertex()[i] << " | " << d[i] << endl;
		}
	}
}