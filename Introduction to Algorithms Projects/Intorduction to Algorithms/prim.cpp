/// Prim最小生成树算法



#include <vector>
#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <iterator>
#include <bitset>
#include <queue>
#include "graphics.h"

using namespace std;
using namespace chapter22;

namespace chapter23
{
	void Prim()
	{
		cout << "Prim最小生成树" << endl;
		vector<char> v;
		for (int i = 0; i < 9; ++i)
		{
			v.push_back('a' + i);
		}
		GrpahicsViaAdjacencyMatrix<char, int> g(v, Undigraph);
		g.Link2Vertex(0, 1, 4);
		g.Link2Vertex(0, 7, 8);
		g.Link2Vertex(1, 2, 8);
		g.Link2Vertex(1, 7, 11);
		g.Link2Vertex(2, 3, 7);
		g.Link2Vertex(2, 5, 4);
		g.Link2Vertex(2, 8, 2);
		g.Link2Vertex(3, 4, 9);
		g.Link2Vertex(3, 5, 14);
		g.Link2Vertex(4, 5, 10);
		g.Link2Vertex(5, 6, 2);
		g.Link2Vertex(6, 7, 1);
		g.Link2Vertex(6, 8, 6);
		g.Link2Vertex(7, 8, 7);
		int start_index = 0;

		//parent[i] = j代表在选取最小生成树时第i个结点的上一个结点是j
		vector<int> parent(v.size(), -1);
		//不在最小生成树中的顶点的集合
		//pair<顶点的编号，该顶点与任意已经在最小生成树中的顶点的最短距离>
		map<int, int> key;
		for (int i = 0; i < g.GetVertex().size(); ++i)
		{
			key.insert(make_pair(i, numeric_limits<int>::max()));
		}
		key[start_index] = 0;

		while (!key.empty())
		{
			//得到与最小生成树中任意顶点距离最小的元素
			auto min_ele = min_element(key.begin(), key.end(), [](pair<int const, int> &p1, pair<int const, int> &p2){
				return p1.second < p2.second;
			});
			int min_index = min_ele->first;
			cout << (parent[min_index] == -1 ? *"&" : g.GetVertex()[parent[min_index]]) << " -- " << g.GetVertex()[min_index] << endl;
			key.erase(min_ele);

			for (int i = 0; i < g.GetVertex().size(); ++i)
			{
				if (key.find(i) != key.end() && g.IsLinked(i, min_index) && key[i] > g.GetEdge()[i][min_index])
				{
					key[i] = g.GetEdge()[i][min_index];
					parent[i] = min_index;
				}
			}
		}

	}
}