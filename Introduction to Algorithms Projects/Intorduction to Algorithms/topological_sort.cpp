//////////////////////////////////////////////////////////////////////////  
/// @file		topological_sort.cpp
/// @brief		拓扑排序
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
/// 2011/06/17   16:03	1.0	谭川奇	创建


#include <vector>
#include <iostream>
#include <queue>
#include <set>
#include <bitset>
#include <string>
#include "graphics.h"
#include "deapth_first_search.h"

using namespace std;

namespace ita
{
	/// 拓扑排序
	void TopologicalSort()
	{
		vector<string> v;
		v.push_back("undershorts");
		v.push_back("socks");
		v.push_back("watch");
		v.push_back("pants");
		v.push_back("shoes");
		v.push_back("shirt");
		v.push_back("belt");
		v.push_back("tie");
		v.push_back("jacket");
		GraphicsViaAdjacencyList<string> g(v, Digraph);
		g.Link2Vertex(0, 3);
		g.Link2Vertex(0, 4);
		g.Link2Vertex(1, 4);
		g.Link2Vertex(3, 4);
		g.Link2Vertex(3, 6);
		g.Link2Vertex(5, 6);
		g.Link2Vertex(5, 7);
		g.Link2Vertex(6, 8);
		g.Link2Vertex(7, 8);

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

		cout << "拓扑排序：" << endl;
		vector<pair<int, pair<int, int>>> r;
		for (size_t i = 0; i < g.GetVertex().size(); ++i)
		{
			r.push_back(make_pair(i, make_pair(d[i], f[i])));
		}
		//根据时间戳f来逆向排好序的结果就是拓扑排序的结果
		sort(r.rbegin(), r.rend(), [](pair<int, pair<int, int>> const &p1, pair<int, pair<int, int>> const &p2){return p1.second.second < p2.second.second;});
		for (size_t i = 0; i < r.size(); ++i)
		{
			cout << g.GetVertex()[r[i].first] << "[" << r[i].second.first << "," << r[i].second.second << "]" << endl;
		}

	}

}