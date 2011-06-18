//////////////////////////////////////////////////////////////////////////
/// @file		bitonic_tour.cpp
/// @brief		双调欧几里得旅行商问题
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
/// 2011/06/17   14:15	1.0	谭川奇	创建

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

namespace ita
{

    /// @brief 双调欧几里得旅行商问题
    ///
    /// 欧几里得旅行商问题是对平面上给定的n个点确定一条连接各点的最短闭合旅程的问题，这个问题的一般形式是NP完全的，故其解需要多于多项式的时间。\n
    /// J.L.Bentley建议通过只考虑双调旅程来简化问题，这种旅程即为从最左点开始，严格地从左到右直至最右点，然后严格地从右到左直至出发点。在这种情况下，多项式时间的算法是可能的。\n
    /// 描述一个确定最优双调路线的O(n^2)时间的算法，可以假设任何两点的x坐标都不相同。\n\n
    /// 参考了这篇Blog http://blog.sina.com.cn/s/blog_41630e7e0100dp4o.html \n
    /// 此文的思想正确，但是在表述上有些小BUG，在此基础上修正而来的\n
    ///
    /// @remarks	找到最优子问题是动态规划的关键--本问题的最优子问题可以描述如下：\n
    ///				将各个节点从左至右进行排序，编号为1，2，....n。	B[i][j]表示从i点到第0点，再折回到第j点的最优双调旅程。这样，B[i][j]的递归最优子问题可以表述为以下三种情况：
    ///				- B[i+1,k]		= B[i,k] + w(i,i+1)
    ///				- B[i+1,i]		= min{ B[u,i] + w(u,i+1) }				(其中 0<=u<=i)
    ///				- B[i+1,i+1]	= min{ B[u,i] + w(u,i+1) + w(i,i+1) }	(其中 0<=u<=i)
    ///	@remarks	理解这三个情况的递推公司最重要的是要明白：双调旅程问题的每一趟都是单调的，要不单增要不单减。正确理解并应用这个限制条件是解题的关键！
    void BitonicTour()
    {
        struct Point
        {
            int X, Y;
            Point( int x, int y ) : X( x ), Y( y ) {}
            Point() : X( 0 ), Y( 0 ) {}
        };

        //初始化值为算法导论上的例子的值
        vector<Point> points;
        points.push_back( Point( 1, 7 ) );
        points.push_back( Point( 2, 1 ) );
        points.push_back( Point( 3, 4 ) );
        points.push_back( Point( 6, 5 ) );
        points.push_back( Point( 7, 2 ) );
        points.push_back( Point( 8, 6 ) );
        points.push_back( Point( 9, 3 ) );

        //任意两点之间的距离
        vector<vector<double>> distances( points.size(), vector<double>( points.size() ) );
        for ( size_t i = 0; i < points.size(); ++i )
        {
            for ( size_t j = 0; j < points.size(); ++j )
            {
                if ( i != j )
                {
                    distances[i][j] = sqrt( pow( points[i].X - points[j].X, 2.0 ) + pow( points[i].Y - points[j].Y, 2.0 ) );
                }
            }
        }

        //B[i][j]表示从i点到第0点，再折回到第j点的最优双调旅程
        vector<vector<double>> B( points.size(), vector<double>( points.size(), 0 ) );
        for ( size_t i = 1; i < points.size(); ++i )
        {
            B[0][i] = B[0][i - 1] + distances[i - 1][i];
            B[i][0] = B[0][i];
        }

        for ( size_t i = 1; i < points.size(); ++i )
        {
            for ( size_t j = i; j < points.size(); ++j )
            {
                //进入到这里有一个很重要的先决条件：
                //B[<=i][<=j]的所有值已经被计算出来了，可以用来递推B[i][j]
                //明白了这一点，再加了明白双调旅程是单调的，每一趟的下标必须是递增或者递减就可以完整的明白问题。

                if ( i == j )
                {
                    //这里是i刚向前递推进1的分支
                    vector<double> vd;
                    for ( size_t k = 0; k < i; ++k )
                    {
                        vd.push_back( distances[j][k] + B[k][j] );
                    }
                    B[i][j] = *min_element( vd.begin(), vd.end() );
                }
                else if ( j == i + 1 )
                {
                    vector<double> vd;
                    for ( size_t k = 0; k < i; ++k )
                    {
                        vd.push_back( B[i][k] + distances[k][j] );
                    }
                    B[i][j] = *min_element( vd.begin(), vd.end() );
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