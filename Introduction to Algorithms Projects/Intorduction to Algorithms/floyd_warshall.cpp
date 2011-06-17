//////////////////////////////////////////////////////////////////////////  
/// @file		floyd_warshall.cpp
/// @brief		FloydWarshall每对顶点间的最短路径算法
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
/// 2011/06/17   17:42	1.0	谭川奇	创建

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
	/// @brief FloydWarshall每对顶点间的最短路径算法
	/// 
	/// Folyd-Warshall是一个动态规划算法，运行时间为O(V<sup>3</sup>)，它允许权值为负的边，但是假定了不存在权值为负的回路。\n
	/// Folyd-Warshall的核心在于：相对于“朴素动态规划算法”，它改进了“最优子问题结构”，使用d<sub>ij</sub>(k)来表示从顶点i到顶点j、
	/// 且满足所有中间顶点皆属于集合{1,2,…,k}的一条最短路径的权值。这种限定了起始点的技巧大大的减少了实现的计算量。\n
    void FloydWarshall()
    {
        cout << "FloydWarshall最短路径" << endl;
        //生成P384页的图25-1
        vector<int> v;
        v.push_back( 1 );
        v.push_back( 2 );
        v.push_back( 3 );
        v.push_back( 4 );
        v.push_back( 5 );
        GrpahicsViaAdjacencyMatrix<int, int> g( v, Digraph );
        g.Link2Vertex( 0, 1, 3 );
        g.Link2Vertex( 0, 2, 8 );
        g.Link2Vertex( 0, 4, -4 );
        g.Link2Vertex( 1, 4, 7 );
        g.Link2Vertex( 1, 3, 1 );
        g.Link2Vertex( 2, 1, 4 );
        g.Link2Vertex( 3, 2, -5 );
        g.Link2Vertex( 3, 0, 2 );
        g.Link2Vertex( 4, 3, 6 );

        int const n = v.size();	//顶点的个数
        vector<vector<vector<int>>> D( n + 1, vector<vector<int>>( n, vector<int>( n, 0 ) ) );

        for ( int i = 0; i < n; ++i )
        {
            for ( int j = 0; j < n; ++j )
            {
                D[0][i][j] = numeric_limits<int>::max();
                if ( i == j )
                {
                    D[0][i][j] = 0;
                }
                if ( g.GetEdge()[i][j] != 0 )
                {
                    D[0][i][j] = g.GetEdge()[i][j];
                }
            }
        }

        for ( int k = 1; k <= n; ++k )
        {
            //中间结点允许为[0 ... k-1]的情况
            for ( int i = 0; i < n; ++i )
            {
                for ( int j = 0; j < n; ++j )
                {
                    D[k][i][j] = min( D[k - 1][i][j]
                                      , ( D[k - 1][i][k - 1] == numeric_limits<int>::max() || D[k - 1][k - 1][j] == numeric_limits<int>::max()
                                          ? numeric_limits<int>::max()
                                          : D[k - 1][i][k - 1] + D[k - 1][k - 1][j] ) );
                }
            }
        }

        for ( int i = 0; i < n; ++i )
        {
            for ( int j = 0; j < n; ++j )
            {
                cout << D[n][i][j] << "  ";
            }
            cout << endl;
        }
    }
}