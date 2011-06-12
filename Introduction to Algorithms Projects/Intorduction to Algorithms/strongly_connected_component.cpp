/// 强连通分支

#include "stdafx.h"

#include <vector>
#include <iostream>
#include <queue>
#include <set>
#include <iterator>
#include <bitset>
#include "graphics.h"

using namespace std;


namespace chapter22
{
	template<typename T>
	vector<int> DFS_Visit(GraphicsViaAdjacencyList<T> &g, size_t index, vector<int> &d, vector<int> &f, vector<bool> &traversed, int &time)
	{
		vector<int> this_time_traversed;
		this_time_traversed.push_back(index);
		traversed[index] = true;
		++time;
		d[index] = time;

		for (size_t i = 0; i < g.GetVertex().size(); ++i)
		{
			if (!traversed[i] && g.IsLinked(index, i).first)
			{
				vector<int> v = DFS_Visit(g, i, d, f, traversed, time);
				this_time_traversed.insert(this_time_traversed.end(), v.begin(), v.end());
			}
		}

		++time; 
		f[index] = time;
		return this_time_traversed;
	}


	void StronglyConnectedComponent()
	{
		cout << "强连接分支" << endl;
		vector<char> v;
		for (int i = 0; i < 8; ++i)
		{
			v.push_back('a' + i);
		}
		GraphicsViaAdjacencyList<char> g(v, Digraph);
		g.Link2Vertex(0, 1);
		g.Link2Vertex(1, 2);
		g.Link2Vertex(1, 4);
		g.Link2Vertex(1, 5);
		g.Link2Vertex(2, 3);
		g.Link2Vertex(2, 6);
		g.Link2Vertex(3, 2);
		g.Link2Vertex(3, 7);
		g.Link2Vertex(4, 0);
		g.Link2Vertex(4, 5);
		g.Link2Vertex(5, 6);
		g.Link2Vertex(6, 5);
		g.Link2Vertex(6, 7);
		g.Link2Vertex(7, 7);

		//step1:对g进行深度优先遍历
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

		//step2:得到gT并得到按时间戳f逆向排序
		GraphicsViaAdjacencyList<char> gT(g);
		for (size_t i = 0; i < gT.GetEdge().size(); ++i)
		{
			gT.GetEdge()[i] = NULL;
		}
		for (size_t i = 0; i < g.GetVertex().size(); ++i)
		{
			for (size_t j = 0; j < g.GetVertex().size(); ++j)
			{
				if (g.IsLinked(i, j).first)
				{
					gT.Link2Vertex(j, i);
				}
			}
		}
		vector<pair<int, pair<int, int>>> r;
		for (size_t i = 0; i < g.GetVertex().size(); ++i)
		{
			r.push_back(make_pair(i, make_pair(d[i], f[i])));
		}
		//根据时间戳f来逆向排好序的结果
		sort(r.rbegin(), r.rend(), [](pair<int, pair<int, int>> const &p1, pair<int, pair<int, int>> const &p2){return p1.second.second < p2.second.second;});
		for (size_t i = 0; i < r.size(); ++i)
		{
			cout << g.GetVertex()[r[i].first] << "[" << r[i].second.first << "," << r[i].second.second << "] ";
		}
		cout << endl;
		
		//step3：对gT以以时间戳的逆序来重新进行深度优先遍历，所得到的各子树就是g的强连通分支
		traversed.assign(gT.GetVertex().size(), false);
		time = 0;

		for (size_t i = 0; i < r.size(); ++i)
		{
			if (!traversed[r[i].first])
			{
				vector<int> ids = DFS_Visit(gT, r[i].first, d, f, traversed, time);
				for (size_t k = 0; k < ids.size(); ++k)
				{
					cout << gT.GetVertex()[ids[k]];
				}
				cout << endl;
			}			
		}

	}

}