//////////////////////////////////////////////////////////////////////////
/// @file		breadth_first_search.cpp
/// @brief		广度优先搜索
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
/// 2011/06/17   16:02	1.0	谭川奇	创建


#include <vector>
#include <iostream>
#include <queue>
#include <set>
#include <bitset>
#include "graphics.h"

using namespace std;

namespace ita
{
    /// @brief 广度优先遍历
    ///
    /// @param	g				使用邻接表表示的图
    /// @param	start_index		从这个顶点开始进行广度优先遍历
    template<typename T>
    void BreadthFirstSearch( GraphicsViaAdjacencyList<T> &g, size_t start_index )
    {
        queue<size_t>	q;
        vector<bool>	trivaled( g.GetVertex().size(), false );
        q.push( start_index );
        trivaled[start_index] = true;

        cout << "广度优先遍历：";
        while ( !q.empty() )
        {
            //如果没有遍历完所有的结点
            char current = q.front();
            q.pop();
            cout << g.GetVertex()[current] << "  ";

            for ( size_t i = 0; i < g.GetVertex().size(); ++i )
            {
                if ( !trivaled[i] && g.IsLinked( current, i ).first )
                {
                    //没有被遍历过 且 与当前遍历的元素邻接
                    q.push( i );
                    trivaled[i] = true;
                }
            }
        }
    }

    /// 广度优先遍历
    void testBreadthFirstSearch()
    {
        vector<char> v;
        for ( int i = 0; i < 8; ++i )
        {
            v.push_back( 'r' + i );
        }
        GraphicsViaAdjacencyList<char> g( v, Undigraph );
        g.Link2Vertex( 0, 1 );
        g.Link2Vertex( 0, 4 );
        g.Link2Vertex( 1, 5 );
        g.Link2Vertex( 2, 5 );
        g.Link2Vertex( 2, 3 );
        g.Link2Vertex( 2, 6 );
        g.Link2Vertex( 3, 6 );
        g.Link2Vertex( 3, 7 );
        g.Link2Vertex( 5, 6 );
        g.Link2Vertex( 6, 7 );

        BreadthFirstSearch( g, 1 );
        cout << endl;
    }
}