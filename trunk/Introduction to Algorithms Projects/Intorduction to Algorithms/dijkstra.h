#pragma once

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
using namespace chapter22;

namespace chapter24
{
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