/// 强连通分支

#include "stdafx.h"

#include <vector>
#include <iostream>
#include <queue>
#include <set>
#include <bitset>
#include "graphics.h"

using namespace std;


namespace chapter22
{
	template<typename T>
	void DFS_Visit(GraphicsViaAdjacencyList<T> &g, size_t index, vector<int> &d, vector<int> &f, vector<bool> &traversed, int &time);


	void StronglyConnectedComponent()
	{
		vector<char> v;
		for (int i = 0; i < 8; ++i)
		{
			v.push_back('r' + i);
		}
		GraphicsViaAdjacencyList<char> g(v, Undigraph);
		g.Link2Vertex(0, 1);
		g.Link2Vertex(0, 4);
		g.Link2Vertex(1, 5);
		g.Link2Vertex(2, 5);
		g.Link2Vertex(2, 3);
		g.Link2Vertex(2, 6);
		g.Link2Vertex(3, 6);
		g.Link2Vertex(3, 7);
		g.Link2Vertex(5, 6);
		g.Link2Vertex(6, 7);

		//step1:进行深度优先遍历
		vector<int>		d(g.GetVertex().size());				//时间戳d
		vector<int>		f(g.GetVertex().size());				//时间戳f
		vector<bool>	traversed(g.GetVertex().size(), false);	//标识顶点是否已经被遍历
		int				time = 0;

		for (size_t i = 0; i < g.GetVertex().size(); ++i)
		{
			if (!traversed[i])
			{
				DFS_Visit(g, i, d, f, traversed, time);
			}			
		}

		//step2:得到gT并按时间戳f逆向排序
		GraphicsViaAdjacencyList<char> gT(g);
		for (size_t i = 0; i < gT.GetEdge().size(); ++i)
		{
			gT.GetEdge()[i] = NULL;
		}
		for (size_t i = 0; i < g.GetVertex().size(); ++i)
		{
			for (size_t j = 0; j < g.GetVertex().size(); ++j)
			{
				if (g.IsLinked(i, j))
				{
					gT.Link2Vertex(j, i);
				}
			}
		}
		for (size_t i = 0; i < gT.GetVertex().size() - 1; ++i)
		{
			for (size_t j = i + 1; j < gT.GetEdge().size(); ++i)
			{
				if (f[i] < f[j])
				{
					swap(gT.GetVertex()[i], gT.GetVertex()[j]);
					swap(gT.GetEdge()[i], gT.GetEdge()[j]);
				}
			}
		}
		
		//step3：对gT以

	}

}