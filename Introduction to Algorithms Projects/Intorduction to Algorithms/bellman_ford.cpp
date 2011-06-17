//////////////////////////////////////////////////////////////////////////  
/// @file		bellman_ford.cpp
/// @brief		Bellman-Ford最短路径算法
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
/// 2011/06/17   17:40	1.0	谭川奇	创建

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
#include "bellman_ford.h"

using namespace std;

namespace ita
{
	/// BellmanFord单源最短路径算法
	bool testBellmanFord()
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

		if (!BellmanFord(g, start_index, d, parent_index))
		{
			cout << "存在负值回路" << endl;
		}
		else
		{
			for (size_t i = 0; i < g.GetVertex().size(); ++i)
			{
				cout << g.GetVertex()[i] << " | " << d[i] << endl;
			}
		}

		return true;
	}
}