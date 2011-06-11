/// Kruskal最小生成树算法

#include "stdafx.h"

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

		vector<DisjointSetForest::DisjointSet<char> *> dsf;
		for (int i = 0; i < g.GetVertex().size(); ++i)
		{
			dsf.push_back(DisjointSetForest::MakeSet(g.GetVertex()[i]));
		}

		// 最小生成树一定是N个结点，N-1条边
		for (int i = 0; i < g.GetVertex().size() - 1; ++i)
		{
			vector<pair<int, pair<int, int>>> vw;
			for (int i = 0; i < g.GetVertex().size(); ++i)
			{
				for (int j = 0; j < g.GetVertex().size(); ++j)
				{
					if (g.IsLinked(i, j) && DisjointSetForest::FindSet(dsf[i]) != DisjointSetForest::FindSet(dsf[j]))
					{
						vw.push_back(make_pair(g.GetEdge()[i][j], make_pair(i, j)));
					}
				}
			}
			sort(vw.begin(), vw.end());
			DisjointSetForest::Union(dsf[vw[0].second.first], dsf[vw[0].second.second]);
			cout << vw[0].second.first << "  -->  " << vw[0].second.second << endl;
		}

		for_each(dsf.begin(), dsf.end(), [](DisjointSetForest::DisjointSet<char> *adsf){delete adsf;});
	}
}