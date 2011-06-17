//////////////////////////////////////////////////////////////////////////  
/// @file		deapth_first_search.cpp
/// @brief		深度优先搜索
/// @details	COPYRIGHT NOTICE  
///			    Copyright (c) 2011
///			    All rights reserved.\n
///			    
///  
/// @author		谭川奇	chuanqi.tan(at)gmail.com
/// @date		2011/06/17
/// @version	1.0 
//////////////////////////////////////////////////////////////////////////  
/// 修改记录：
/// 2011/06/17   16:56	1.0	谭川奇	创建


#include <vector>
#include <iostream>
#include <queue>
#include <set>
#include <bitset>
#include "graphics.h"
#include "deapth_first_search.h"

using namespace std;

namespace ita
{
	/// @brief 深度优先遍历主函数
	/// 
	/// 深度优先搜索除了创建一个深度优先森林外，深度优先搜索同时为每个顶点加盖时间戳。每个顶点v有两个时间戳：\n
	/// 当顶点v第一次被发现时，记录下第一个时间戳d[v]，当结束检查v的邻接表时，记录下第二个时间戳f[v]。\n
	/// 许多基于深度优先搜索的图算法都用到了时间戳，它们对推算深度优先搜索的时行情况有很大的帮助。\n
	/// 这次重新复习深度优先算法，得到的最大的启示就是使用了这2个时间戳，真的是很有用很好的创新啊。当记录下这2个时间戳之后，很多东西都可以由这对时间戳来推导出来了（比如拓扑排序、深度遍历的次序等）。\n
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

	/// 测试深度优先搜索
	void testDeapthFirstSearch()
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

		DeapthFirstSearch(g);
	}
}