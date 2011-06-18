//////////////////////////////////////////////////////////////////////////
/// @file		dijkstra.cpp
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
#include "dijkstra.h"

using namespace std;


namespace ita
{
    /// Dijkstra单源最短路径算法
    void testDijkstra()
    {
        cout << "Dijkstra最短路径" << endl;
        //生成P367页的图24-6
        vector<char> v;
        v.push_back( 's' );
        v.push_back( 't' );
        v.push_back( 'x' );
        v.push_back( 'z' );
        v.push_back( 'y' );
        GraphicsViaAdjacencyList<char> g( v, Digraph );
        g.Link2Vertex( 0, 1, 10 );
        g.Link2Vertex( 0, 4, 5 );
        g.Link2Vertex( 1, 2, 1 );
        g.Link2Vertex( 1, 4, 2 );
        g.Link2Vertex( 2, 3, 4 );
        g.Link2Vertex( 3, 2, 6 );
        g.Link2Vertex( 3, 0, 7 );
        g.Link2Vertex( 4, 1, 3 );
        g.Link2Vertex( 4, 2, 9 );
        g.Link2Vertex( 4, 3, 2 );

        int start_index = 0;
        vector<int> d( g.GetVertex().size() );
        vector<int> parent_index( g.GetVertex().size() );
        Dijkstra( g, start_index, d, parent_index );


        for ( size_t i = 0; i < g.GetVertex().size(); ++i )
        {
            cout << g.GetVertex()[i] << " | " << d[i] << endl;
        }
    }
}