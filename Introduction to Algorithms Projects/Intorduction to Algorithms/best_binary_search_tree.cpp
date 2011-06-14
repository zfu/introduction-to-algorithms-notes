//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2009
///    All rights reserved.
///
/// @file		best_binary_search_tree.cpp
/// @brief		best_binary_search_tree.cpp的简短描述
///
///  			best_binary_search_tree.cpp的详细描述
///
/// @author		谭川奇	chuanqi.tan(at)gmail.com
/// @date		2011/06/01
/// @version 	1.0
///
///
///    修订说明：最初版本
//////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <iomanip>
#include <limits>
#include <fstream>
using namespace std;

namespace chapter15
{
	/// 最优二叉查找树
	void BestBinarySearchTree()
	{
		//p1 ... p5：这里的p0是个占位符，为了后面的编码方便，p0是无意义的
		double p[] = {0, 0.15, 0.1, 0.05, 0.1, 0.2};
		double q[] = {0.05, 0.1, 0.05, 0.05, 0.05, 0.1};

		//e[i][j]表示从pi -- pj的最优二叉查找树的期望查找代价
		vector< vector<double> > e(7, vector<double>(6));
		//w[i][j]表示从pi -- pj的概率总和
		vector< vector<double> > w(7, vector<double>(6));

		//先计算出叶子结点单独成为一棵树时的期望查找代价
		//当e[i][j]的j<i时，e[i][j]代表的是叶子结点。e[i][i-1]代表的是第i-1个叶子结点的期望查找代价
		for (int i = 1; i < 7; ++i)
		{
			e[i][i - 1] = q[i - 1];
		}

		//计算从pi -- pj的概率总和，包括普通结点和这里的虚拟叶子结点都要计算
		for (int i = 1; i < 7; ++i)
		{
			for (int j = i - 1; j < 6; ++j)
			{
				for (int k = i; k <= j; ++k)
				{//普通结点
					w[i][j] += p[k];
				}
				for (int k = i - 1; k <= j; ++k)
				{//虚拟叶子结点
					w[i][j] += q[k];
				}
			}
		}

		//这种按长度来进行递推的想法很明显，因为长度是依赖关系
		//欲知道长度为n的子集的最优二叉查找树，必定要先知道长度小于n的所有子集的最优二叉查找树
		for (int length = 1; length <= 5; ++length)
		{//从长度短的子集开始推导长度长的子集
			for (int i = 1; i < 6 - length + 1; ++i)
			{
				//对[i...j]这个子集进行计算它的最优期望查找代价
				int j = i + length - 1;

				vector<double> vs;
				for (int k = i; k <= j; ++k)
				{//i<=k<=j，从k位置将[i...j]进行划分，视pk为[pi...pj]这棵子树的根
					vs.push_back(p[k] + e[i][k - 1] + w[i][k - 1] + e[k + 1][j] + w[k + 1][j]);
				}
				e[i][j] = *min_element(vs.begin(), vs.end());
			}
		}

		//输出p1...p5的最优二叉查找树的期望查找代价
		cout << e[1][5] << endl;
		getchar();
	}
}