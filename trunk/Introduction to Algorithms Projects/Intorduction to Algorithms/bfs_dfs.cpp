/// 广度优先搜索 & 深度优先搜索

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
	/// 广度优先遍历
	template<typename T>
	void BreadthFirstSearch(GraphicsViaAdjacencyList<T> &g, size_t start_index)
	{
		queue<size_t>	q;
		vector<bool>	trivaled(g.GetVertex().size(), false);
		q.push(start_index);
		trivaled[start_index] = true;

		cout << "广度优先遍历：";
		while (!q.empty())
		{//如果没有遍历完所有的结点
			char current = q.front();
			q.pop();
			cout << g.GetVertex()[current] << "  ";

			for (size_t i = 0; i < g.GetVertex().size(); ++i)
			{
				if (!trivaled[i] && g.IsLinked(current, i).first)
				{
					//没有被遍历过 且 与当前遍历的元素邻接
					q.push(i);
					trivaled[i] = true;
				}
			}
		}
	}

	/// 深度优先遍历的递归程序
	/// 
	/// @return		返回本次深度优先遍历得到的子树依次所遍历过的结点
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

	/// 深度优先遍历
	template<typename T>
	void DeapthFirstSearch(GraphicsViaAdjacencyList<T> &g)
	{
		vector<int>		d(g.GetVertex().size());				//时间戳d
		vector<int>		f(g.GetVertex().size());				//时间戳f
		vector<bool>	traversed(g.GetVertex().size(), false);	//标识顶点是否已经被遍历
		int				time = 0;
		
		cout << endl << "深度优先遍历：";
		for (size_t i = 0; i < g.GetVertex().size(); ++i)
		{
			if (!traversed[i])
			{
				vector<int> ids = DFS_Visit(g, i, d, f, traversed, time);
				for (size_t k = 0; k < ids.size(); ++k)
				{
					cout << g.GetVertex()[ids[k]] << "[" << d[ids[k]] << "," << f[ids[k]] << "] ";
				}
			}			
		}

		cout << endl << "深度优先遍历：";
		vector<pair<int, pair<int, int>>> r;
		for (size_t i = 0; i < g.GetVertex().size(); ++i)
		{
			r.push_back(make_pair(i, make_pair(d[i], f[i])));
		}
		//根据时间戳d来排序好好序之后就是深度优先遍历的结果
		//这是我自己推导出来的，我的证明思路是：根据深度优先的“括号定理”，时间戳d排在前面的结点总是位于前面的括号内，也就是位于深度优先搜索结果的前列
		//所以说这种同时计算时间界的深度优先真有用啊，根据d排序就可以得出深度优先的次序，根据f排序又可以得出拓扑排序的顺序
		sort(r.begin(), r.end(), [](pair<int, pair<int, int>> const &p1, pair<int, pair<int, int>> const &p2){return p1.second.first < p2.second.first;});
		for (size_t i = 0; i < r.size(); ++i)
		{
			cout << g.GetVertex()[r[i].first] << "[" << r[i].second.first << "," << r[i].second.second << "] ";
		}
	}



	void BFS_And_DFS()
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

		BreadthFirstSearch(g, 1);
		cout << endl;
		DeapthFirstSearch(g);
		cout << endl;
	}
}