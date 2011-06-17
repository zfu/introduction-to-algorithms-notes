#pragma once
//////////////////////////////////////////////////////////////////////////  
/// @file		dijkstra.h
/// @brief		Dijstra最短路径
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
/// 2011/06/17   17:39	1.0	谭川奇	创建


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
	/// @brief Dijstra最短路径
	/// 
	/// <b>最短路径：一个顶点到另一个顶点的最短权值路径。广度优先搜索算法就是一种在无权（单位权值）图上执行的最短路径算法。</b>\n
	/// Dijkstra算法是一种贪心策略的算法，它的运行时间一般比Bellman-Ford算法要好。\n
	///	<span style="color:#FF0000 ">Dijkstra算法在每次循环中，每次仅仅提取d值最小的顶点u是保证这个贪心策略正确性的关键核心所在。</span>\n
	///	因为通过集合S中所有的元素都已经去试着松弛过u了，而非S中的点由于本身它的d值都比u要大，所以即使用它们中的任何一个去松弛u，也不可能达到比现在更小的d值了。\n
	///	因此，在每次循环中选取当前d值最小的顶点加入到S集合中去一定能保证最后得到全局最优解。\n\n
	///	DIJKSTRA(G, w, s)\n
	/// 	1  INITIALIZE-SINGLE-SOURCE(G, s)\n
	/// 	2  S ← 空集\n
	/// 	3  Q ← V[G]\n
	///		4  while Q != 空集\n
	/// 	5      do u ← EXTRACT-MIN(Q)\n
	/// 	6         S ← S 并上 {u}\n
	///		7         for each vertex v 属于 Adj[u]\n
	///		8             do RELAX(u, v, w)\n
	/// @remarks	Dijkstra算法假定输入图中的所有边的权值都是非负的
	/// @param	g				用邻接表法表示的图
	/// @param	start_index		计算顶点start_index到其它所有点的最短路径
	/// @param	d				算法结束后，d[i]存储从start_index到顶点i的最短路径
	/// @parem	parent_index	记录算法在运行的进行中的选。算法结束后，parent_index[i]表示从start_index到顶点i的最短路径的倒数第二个结点的编号
	template<typename T>
	void Dijkstra(GraphicsViaAdjacencyList<T> &g, int start_index, vector<int> &d, vector<int> &parent_index)
	{
		auto edges = g.GetAllEdges();

		InitializeSingleSource(g, d, parent_index, start_index);

		auto greater_pred = [&](int index1, int index2){
			return d[index1] > d[index2];
		};

		//使用普通二叉堆来做这个优先队列。还可以考虑使用二项堆和斐波那契堆来进行加速
		vector<int> Q;
		for (size_t i = 0; i < g.GetVertex().size(); ++i)
		{
			Q.push_back(i);
		}
		make_heap(Q.begin(), Q.end(), greater_pred);


		while(!Q.empty())
		{
			for_each(edges.begin(), edges.end(), [&](pair<size_t, size_t> const &p){
				if (p.first == Q[0])
				{
					//对所有从min_ele发出的边进行松弛操作
					Relax(g, d, parent_index, p.first, p.second);
				}
			});

			//这里d被Relax进行了改变，相当于改变了优先队列中若干项的优先值，因此要重建堆
			swap(Q[0], Q[Q.size() - 1]); Q.pop_back();
			make_heap(Q.begin(), Q.end(), greater_pred);
		}
	}
}