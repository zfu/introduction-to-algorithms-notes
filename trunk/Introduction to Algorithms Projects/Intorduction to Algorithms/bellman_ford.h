#pragma once
//////////////////////////////////////////////////////////////////////////  
/// @file		bellman_ford.h
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

using namespace std;


namespace ita
{
	namespace 
	{
		/// 初始化
		template<typename T>
		void InitializeSingleSource(GraphicsViaAdjacencyList<T> &g, vector<int> &d, vector<int> &parent_index, int start_index)
		{
			for (size_t i = 0; i < g.GetVertex().size(); ++i)
			{
				d[i] = numeric_limits<int>::max();
				parent_index[i] = -1;
			}
			d[start_index] = 0;
		}

		/// 松弛
		/// 
		/// 前提条件是：index1->index2存在着一条边
		/// 因此用index1的这条边来松弛index2的d值，并且设置parent[index2] = index1
		template<typename T>
		void Relax(GraphicsViaAdjacencyList<T> &g, vector<int> &d, vector<int> &parent_index, int index1, int index2)
		{
			if (d[index2] > d[index1] + g.IsLinked(index1, index2).second->Weight)
			{
				d[index2] = d[index1] + g.IsLinked(index1, index2).second->Weight;
				parent_index[index2] = index1;
			}
		}
	}

	/// @brief Bellman-Ford最短路径算法
	/// 
	/// <b>最短路径：一个顶点到另一个顶点的最短权值路径。广度优先搜索算法就是一种在无权（单位权值）图上执行的最短路径算法。</b>\n
	/// Bellman-Ford算法非常简单：对所有的边进行|v|-1遍循环，在每次循环中对每一条边进行松弛的操作。\n
	/// @remarks		floyd算法允许输入边存在负权边，只要不存在从源点可达的负权回路。而且如果存在着负权回路，它还能检测出来。
	/// @param	g				用邻接表法表示的图
	/// @param	start_index		计算顶点start_index到其它所有点的最短路径
	/// @param	d				算法结束后，d[i]存储从start_index到顶点i的最短路径
	/// @parem	parent_index	记录算法在运行的进行中的选。算法结束后，parent_index[i]表示从start_index到顶点i的最短路径的倒数第二个结点的编号
	/// @return			算法是否执行成功，取决于图中是否存在“负权回路”。
	/// @retval	true	算法执行成功，图中不存在“负权回路
	/// @retval	false	算法执行失败，图中存在“负权回路
	template<typename T>
	bool BellmanFord(GraphicsViaAdjacencyList<T> &g, int start_index, vector<int> &d, vector<int> &parent_index)
	{
		auto edges = g.GetAllEdges();

		InitializeSingleSource(g, d, parent_index, start_index);		

		for (size_t i = 0; i < g.GetVertex().size() - 1; ++i)
		{			
			for_each(edges.begin(), edges.end(), [&](pair<size_t, size_t> const &p){
				Relax(g, d, parent_index, p.first, p.second);
			});
		}

		for (size_t i = 0; i < edges.size(); ++i)
		{
			if (d[edges[i].second] > d[edges[i].first] + g.IsLinked(edges[i].first, edges[i].second).second->Weight)
			{
				return false;
			}
		}

		return true;
	}
}