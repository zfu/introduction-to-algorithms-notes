/// Kruskal最小生成树算法



#include <vector>
#include <iostream>
#include <queue>
#include <set>
#include <iterator>
#include <bitset>
#include "graphics.h"
#include "disjoint_set_forest.h"

using namespace std;
using namespace chapter22;
using namespace chapter21;

namespace chapter23
{
	void Kruskal()
	{
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
		int const n = g.GetVertex().size();

		//建立这些查并集
		vector<DisjointSetForest::DisjointSet<char> *> dsf;
		for (int i = 0; i < g.GetVertex().size(); ++i)
		{
			dsf.push_back(DisjointSetForest::MakeSet(g.GetVertex()[i]));
		}

		//取得边的集合
		vector<pair<int, int>> edges;
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				if (g.IsLinked(i, j))
				{
					edges.push_back(make_pair(i, j));
				}
			}
		}
		//将边的集合按权值进行排序
		sort(edges.begin(), edges.end(), [&g](pair<int, int> &p1, pair<int, int> &p2){
			return g.GetEdge()[p1.first][p1.second] < g.GetEdge()[p2.first][p2.second];
		});

		//依次检查这些边
		for_each(edges.begin(), edges.end(), [&](pair<int, int> &edge){
			if (DisjointSetForest::FindSet(dsf[edge.first]) != DisjointSetForest::FindSet(dsf[edge.second]))
			{
				DisjointSetForest::Union(dsf[edge.first], dsf[edge.second]);
				cout << edge.first << "  -->  " << edge.second << endl;
			}
		});

		for_each(dsf.begin(), dsf.end(), [](DisjointSetForest::DisjointSet<char> *adsf){delete adsf;});
	}
}