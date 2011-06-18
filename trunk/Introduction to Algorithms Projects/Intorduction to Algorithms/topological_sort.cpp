//////////////////////////////////////////////////////////////////////////
/// @file		topological_sort.cpp
/// @brief		拓扑排序
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
#include <bitset>
#include <string>
#include "graphics.h"
#include "deapth_first_search.h"

using namespace std;

namespace ita
{
    /// @breif 拓扑排序
    ///
    /// 拓扑排序：在深度优先遍历的基础上，对有向无回路图进行拓扑排序简直是小菜一蹀。根据遍历所得到的时间戳f[i]逆向排序就好了。\n
    /// 拓扑排序的顶点以与其完成时间时间相反的顺序出现。这种新方法真是长见识啊，比我以前使用的方法好多了，这种方法在遍历完只需要一个简单的sort就完成了拓扑排序，时间复杂度也降低为O(V+E)。\n
    /// 这种新的拓扑排序的方法的理论基础是：对于任一对不同的顶点u,v，如果存在一条从u -> v的边，那么u在拓扑排序的结果中一定在v的前面。\n
    /// 而又根据后裔区间嵌套的定理：如果存在u -> v，那么f[v]<f[u]，所以得证根据f逆向排序得到的顺序一定为拓扑排序。
    void TopologicalSort()
    {
        vector<string> v;
        v.push_back( "undershorts" );
        v.push_back( "socks" );
        v.push_back( "watch" );
        v.push_back( "pants" );
        v.push_back( "shoes" );
        v.push_back( "shirt" );
        v.push_back( "belt" );
        v.push_back( "tie" );
        v.push_back( "jacket" );
        GraphicsViaAdjacencyList<string> g( v, Digraph );
        g.Link2Vertex( 0, 3 );
        g.Link2Vertex( 0, 4 );
        g.Link2Vertex( 1, 4 );
        g.Link2Vertex( 3, 4 );
        g.Link2Vertex( 3, 6 );
        g.Link2Vertex( 5, 6 );
        g.Link2Vertex( 5, 7 );
        g.Link2Vertex( 6, 8 );
        g.Link2Vertex( 7, 8 );

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

        cout << "拓扑排序：" << endl;
        vector<pair<int, pair<int, int>>> r;
        for ( size_t i = 0; i < g.GetVertex().size(); ++i )
        {
            r.push_back( make_pair( i, make_pair( d[i], f[i] ) ) );
        }
        //根据时间戳f来逆向排好序的结果就是拓扑排序的结果
        sort( r.rbegin(), r.rend(), []( pair<int, pair<int, int>> const & p1, pair<int, pair<int, int>> const & p2 )
        {
            return p1.second.second < p2.second.second;
        } );
        for ( size_t i = 0; i < r.size(); ++i )
        {
            cout << g.GetVertex()[r[i].first] << "[" << r[i].second.first << "," << r[i].second.second << "]" << endl;
        }

    }

}