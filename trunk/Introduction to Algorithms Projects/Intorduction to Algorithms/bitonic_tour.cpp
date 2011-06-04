#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <iomanip>
#include <limits>
#include <fstream>
#include <string>
#include <numeric>
#include <set>
#include <cmath>
using namespace std;

namespace chapter15
{
	struct Point
	{
		int X, Y;
		Point(int x, int y) : X(x), Y(y){}
		Point() : X(0), Y(0){}
	};

	/// 双调欧几里得旅行商问题
	/// 
	/// 参考了这篇Blog http://blog.sina.com.cn/s/blog_41630e7e0100dp4o.html 
	/// 此文的思想正确，但是在表述上有些小BUG，在此基础上修正而来的
	/// 此问题最重要的是要明白：双调旅程问题的每一趟都是单调的，要不单增要不单减。正确理解并应用这个限制条件是解题的关键！
	void BitonicTour()
	{
		//初始化值为算法导论上的例子的值
		vector<Point> points;
		points.push_back(Point(1, 7));
		points.push_back(Point(2, 1));
		points.push_back(Point(3, 4));
		points.push_back(Point(6, 5));
		points.push_back(Point(7, 2));
		points.push_back(Point(8, 6));
		points.push_back(Point(9, 3));

		//任意两点之间的距离
		vector<vector<double>> distances(points.size(), vector<double>(points.size()));
		for (size_t i = 0; i < points.size(); ++i)
		{
			for (size_t j = 0; j < points.size(); ++j)
			{
				if (i != j)
				{
					distances[i][j] = sqrt(pow(points[i].X - points[j].X, 2.0) + pow(points[i].Y - points[j].Y, 2.0));
				}
			}
		}

		//B[i][j]表示从i点到第0点，再折回到第j点的最优双调旅程
		vector<vector<double>> B(points.size(), vector<double>(points.size(), 0));
		for (size_t i = 1; i < points.size(); ++i)
		{
			B[0][i] = B[0][i - 1] + distances[i - 1][i];
			B[i][0] = B[0][i];
		}

		for (size_t i = 1; i < points.size(); ++i)
		{
			for (size_t j = i; j < points.size(); ++j)
			{
				//进入到这里有一个很重要的先决条件：
				//B[<=i][<=j]的所有值已经被计算出来了，可以用来递推B[i][j]
				//明白了这一点，再加了明白双调旅程是单调的，每一趟的下标必须是递增或者递减就可以完整的明白问题。

				if (i == j)
				{//这里是i刚向前递推进1的分支
					vector<double> vd;
					for (size_t k = 0; k < i; ++k)
					{
						vd.push_back( distances[j][k] + B[k][j] );
					}
					B[i][j] = *min_element(vd.begin(), vd.end());
				}
				else if (j == i + 1)
				{
					vector<double> vd;
					for (size_t k = 0; k < i; ++k)
					{
						vd.push_back( B[i][k] + distances[k][j] );
					}
					B[i][j] = *min_element(vd.begin(), vd.end());
				}
				else
				{
					B[i][j] = B[i][j - 1] + distances[j - 1][j];
				}

				B[j][i] = B[i][j];
			}
		}

		cout << B[points.size() - 1][points.size() - 1] << endl;
	}
}