//////////////////////////////////////////////////////////////////////////
/// @file		strongly_connected_component.cpp
/// @brief		强连通分支
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
#include <iterator>
#include <bitset>
#include "graphics.h"
#include "deapth_first_search.h"

using namespace std;


namespace ita
{
    /// @brief 寻找强连通分支问题
    ///
    /// 强连通分支：有向图G=(V, E)的一个强连通分支就是一个最大的顶点集合C，对于C中的每一对顶点u,v，都有u -> v及v -> u；亦即顶点u和v是互相可达的。\n
    /// 寻找强连通分支的简明算法，寻找强连通分支的算法的时间复杂度为O(V+E)：
    /// - 对G进行深度优先遍历得到每个顶点的时间戳f[x]；
    /// - 求得G的返回图GT；
    /// - 按照f[x]的逆向顺序为顶点顺序对GT进行深度优先遍历，即按照G的拓扑排序的顺序对GT再进行深度优先遍历；
    /// - 步骤c得到的各棵子树就是原图G的各强连通分支。
    void StronglyConnectedComponent()
    {
        cout << "强连接分支" << endl;
        vector<char> v;
        for ( int i = 0; i < 8; ++i )
        {
            v.push_back( 'a' + i );
        }
        GraphicsViaAdjacencyList<char> g( v, Digraph );
        g.Link2Vertex( 0, 1 );
        g.Link2Vertex( 1, 2 );
        g.Link2Vertex( 1, 4 );
        g.Link2Vertex( 1, 5 );
        g.Link2Vertex( 2, 3 );
        g.Link2Vertex( 2, 6 );
        g.Link2Vertex( 3, 2 );
        g.Link2Vertex( 3, 7 );
        g.Link2Vertex( 4, 0 );
        g.Link2Vertex( 4, 5 );
        g.Link2Vertex( 5, 6 );
        g.Link2Vertex( 6, 5 );
        g.Link2Vertex( 6, 7 );
        g.Link2Vertex( 7, 7 );

        //step1:对g进行深度优先遍历
        vector<int>		d( g.GetVertex().size() );				//时间戳d
        vector<int>		f( g.GetVertex().size() );				//时间戳f
        vector<bool>	traversed( g.GetVertex().size(), false );	//标识顶点是否已经被遍历
        int				time = 0;

        for ( size_t i = 0; i < g.GetVertex().size(); ++i )
        {
            if ( !traversed[i] )
            {
                DFS_Visit( g, i, d, f, traversed, time );
            }
        }

        //step2:得到gT并得到按时间戳f逆向排序
        GraphicsViaAdjacencyList<char> gT( g );
        for ( size_t i = 0; i < gT.GetEdges().size(); ++i )
        {
            gT.GetEdges()[i] = nullptr;
        }
        for ( size_t i = 0; i < g.GetVertex().size(); ++i )
        {
            for ( size_t j = 0; j < g.GetVertex().size(); ++j )
            {
                if ( g.IsLinked( i, j ).first )
                {
                    gT.Link2Vertex( j, i );
                }
            }
        }
        vector<pair<int, pair<int, int>>> r;
        for ( size_t i = 0; i < g.GetVertex().size(); ++i )
        {
            r.push_back( make_pair( i, make_pair( d[i], f[i] ) ) );
        }
        //根据时间戳f来逆向排好序的结果
        sort( r.rbegin(), r.rend(), []( pair<int, pair<int, int>> const & p1, pair<int, pair<int, int>> const & p2 )
        {
            return p1.second.second < p2.second.second;
        } );
        for ( size_t i = 0; i < r.size(); ++i )
        {
            cout << g.GetVertex()[r[i].first] << "[" << r[i].second.first << "," << r[i].second.second << "] ";
        }
        cout << endl;

        //step3：对gT以以时间戳的逆序来重新进行深度优先遍历，所得到的各子树就是g的强连通分支
        traversed.assign( gT.GetVertex().size(), false );
        time = 0;

        for ( size_t i = 0; i < r.size(); ++i )
        {
            if ( !traversed[r[i].first] )
            {
                vector<int> ids = DFS_Visit( gT, r[i].first, d, f, traversed, time );
                for ( size_t k = 0; k < ids.size(); ++k )
                {
                    cout << gT.GetVertex()[ids[k]];
                }
                cout << endl;
            }
        }

    }

}