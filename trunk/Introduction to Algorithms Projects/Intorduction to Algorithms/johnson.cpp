//////////////////////////////////////////////////////////////////////////
/// @file		johnson.cpp
/// @brief		Johnson每对顶点间的最短路径算法
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
#include "bellman_ford.h"
#include "dijkstra.h"

using namespace std;



namespace ita
{
    /// @brief Johnson每对顶点间的最短路径算法
    ///
    /// Johnson算法可在O(V<sup>2</sup>lgV+VE)时间内，求出每对顶点间的最短路径。Johnson算法使用Dijkstra和Bellman-Ford算法作为其子程序。	\n
    /// Johnson算法在所有的边为非负时，把每对顶点依次作为源点来执行Dijkstra算法，就可以找到每对顶点间的最短路径；利用斐波那契最小优先队列，该算法的运行时间为O(V<sup>2</sup>lgV+VE)。\n
    /// <b>因此也可以总结出：对于确定无负权值的图，直接循环调用Dijkstra算法就是求每对顶点间最短路径的最佳算法。</b>\n
    /// Johnson算法使用了重赋权技术来处理负边权值，对每一条边的权值w赋予一个新的权值w’，使用新的边权值集合满足以下两个性质：
    /// - 对所有的顶点u,v，如果路径p是在权值函数w下从u到v的最短路径，当且仅当p也是在权值函数w’下从u到v的最短路径；
    /// - 对于所有的边u,v，新的权值w’(u,v)是非负的（很明显，这样就满足Dijkstra算法的要求，因此又回到重复调用Dijkstra算法来实现求每对顶点间最短路径）。
    ///
    /// Johnson算法的简明步骤：
    /// - 生成一个新图G’，G’是在G上扩展一个起始点后的结果；
    /// - 在G’上调用Bellman-Ford算法。由于Bellman-Ford算法能够检测负权回路，如果存在负权回路则报告存在负权回路并结束整个算法；否则得到在G’上调用Bellman-Ford算法得到的h(x)函数；
    /// - 根据h(x)函数对G中的每一条边进行重赋权，使得G中的每一条边都是非负的；
    /// - 对重赋权后的G进行循环调用Dijkstra算法，得到每对顶点间的最短路径；
    /// - 对得到的每对顶点间的最短路径再根据h(x)函数反向构造出原来权值下的最短路径值。
    /// @remarks		Johnson算法是一个<b>实际上非常好</b>的算法，它使得所有的情况（可能存在负权值和负权回路）都可以使用最好的Dijkstra算法来达到最好的运行效率。
    void Johnson()
    {
        cout << "Johnson最短路径" << endl;
        //生成P384页的图25-1
        vector<int> v;
        v.push_back( 1 );
        v.push_back( 2 );
        v.push_back( 3 );
        v.push_back( 4 );
        v.push_back( 5 );
        GraphicsViaAdjacencyList<int> g( v, Digraph );
        g.Link2Vertex( 0, 1, 3 );
        g.Link2Vertex( 0, 2, 8 );
        g.Link2Vertex( 0, 4, -4 );
        g.Link2Vertex( 1, 4, 7 );
        g.Link2Vertex( 1, 3, 1 );
        g.Link2Vertex( 2, 1, 4 );
        g.Link2Vertex( 3, 2, -5 );
        g.Link2Vertex( 3, 0, 2 );
        g.Link2Vertex( 4, 3, 6 );
        int const n = g.GetVertex().size();

        //generate g'
        GraphicsViaAdjacencyList<int> gplus( v, Digraph );
        for ( int i = 0; i < g.GetVertex().size(); ++i )
        {
            for ( int j = 0; j < g.GetVertex().size(); ++j )
            {
                if ( g.IsLinked( i, j ).first )
                {
                    gplus.Link2Vertex( i, j, g.IsLinked( i, j ).second->Weight );
                }
            }
        }
        gplus.GetVertex().push_back( 0 );
        gplus.GetEdges().push_back( nullptr );
        for ( int i = 0; i < g.GetVertex().size(); ++i )
        {
            gplus.Link2Vertex( gplus.GetVertex().size() - 1, i, 0 );
        }

        //call bellman-ford algorithm on g'
        vector<int> d( gplus.GetVertex().size() );
        vector<int> parent_index( gplus.GetVertex().size() );
        if ( !BellmanFord( gplus, gplus.GetVertex().size() - 1, d, parent_index ) )
        {
            cout << "存在着负值回路，算法无法进行" << endl;
            return;
        }

        //re-assign for g
        auto edges = g.GetAllEdges();
        for ( int i = 0; i < edges.size(); ++i )
        {
            auto node = g.IsLinked( edges[i].first, edges[i].second ).second;
            node->Weight += ( d[edges[i].first] - d[edges[i].second] );
        }

        //loop call dijkstra algorithm on g
        vector<vector<int>> D( n, vector<int>( n ) );
        for ( int i = 0; i < g.GetVertex().size(); ++i )
        {
            vector<int> d2( g.GetVertex().size() );
            vector<int> parent_index2( g.GetVertex().size() );

            Dijkstra( g, i, d2, parent_index2 );

            for ( int j = 0; j < g.GetVertex().size(); ++j )
            {
                D[i][j] = d2[j] + d[j] - d[i];
            }
        }

        //display result
        for ( int i = 0; i < n; ++i )
        {
            for ( int j = 0; j < n; ++j )
            {
                cout << D[i][j] << "  ";
            }
            cout << endl;
        }
    }
}