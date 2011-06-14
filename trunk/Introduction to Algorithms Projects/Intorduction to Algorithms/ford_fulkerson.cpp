#include <vector>
#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <iterator>
#include <bitset>
#include <queue>
#include <algorithm>
#include <numeric>
#include <limits>
#include "graphics.h"

using namespace std;
using namespace chapter22;

namespace chapter26
{
    /// 得到残留网络
    GrpahicsViaAdjacencyMatrix<string, int> GetResidualNetwork( GrpahicsViaAdjacencyMatrix<string, int> &g, vector<vector<int>> &f )
    {
        GrpahicsViaAdjacencyMatrix<string, int> gplus( g.GetVertex(), Digraph );

        for ( int i = 0; i < g.GetVertex().size(); ++i )
        {
            for ( int j = i + 1; j < g.GetVertex().size(); ++j )
            {
                if ( g.IsLinked( i, j ) || g.IsLinked( j, i ) )
                {
                    gplus.Link2Vertex( i, j, g.GetEdge()[i][j] - f[i][j] + f[j][i] );
                    gplus.Link2Vertex( j, i, g.GetEdge()[i][j] + g.GetEdge()[j][i] - gplus.GetEdge()[i][j] );
                }
            }
        }

        return gplus;
    }

    /// 运用广度优先算法得到增广路径
    vector<int> GetAugmentingPath( GrpahicsViaAdjacencyMatrix<string, int> &g )
    {
        vector<int> path;

        //q是一个队列pair<要处理的结点的编号，在该广度优先路径中它的父结点>
        queue<int> q;
        //每一个结点类广度优先遍历中当前的父结点
        vector<int> parent( g.GetVertex().size() );
        //标识一个结点是否被访问过了
        vector<bool>	trivaled( g.GetVertex().size(), false );

        //从s==0的结点开始
        q.push( 0 );
        parent[0] = -1;
        trivaled[0] = true;

        while( !q.empty() )
        {
            int front = q.front();
            for ( int i = 0; i < g.GetVertex().size(); ++i )
            {
                if ( !trivaled[i] && g.IsLinked( front, i ) )
                {
                    q.push( i );
                    parent[i] = front;
                    trivaled[i] = true;

                    if ( i == g.GetVertex().size() - 1 )
                    {
                        //找到了t结点。构造出反向路径
                        int current_index = i;
                        while( current_index != -1 )
                        {
                            path.push_back( current_index );
                            current_index = parent[current_index];
                        }

                        //逆转为正向路径并返回
                        reverse( path.begin(), path.end() );
                        break;
                    }
                }
            }
            q.pop();
        }

        return path;
    }

    /// FordFulkerson最大流算法
    int FordFulkerson( GrpahicsViaAdjacencyMatrix<string, int> &g )
    {
        int const n = g.GetVertex().size();
        vector<vector<int>> f( n, vector<int>( n, 0 ) );

        while ( true )
        {
            //得到残留网络
            auto gplus = GetResidualNetwork( g, f );
            auto a_path = GetAugmentingPath( gplus );
            if ( a_path.empty() )
            {
                //增广路径为空，循环结束
                break;
            }

            vector<int> path_values;
            for ( int i = 0; i < a_path.size() - 1; ++i )
            {
                path_values.push_back( gplus.GetEdge()[a_path[i]][a_path[i + 1]] );
            }
            int min_path_value = *min_element( path_values.begin(), path_values.end() );

            for ( int i = 0; i < a_path.size() - 1; ++i )
            {
                int u = a_path[i];
                int v = a_path[i + 1];

                f[u][v] += min_path_value;

				//算法导论P405页有这一句。但是原理上很明显这一句是不应该有的，而且事实也证明加上这一句就是错误的
				//不解了！也许又是书上的一个bug吧！
                //f[v][u] = -f[u][v];
            }
        }

        return accumulate( f[0].begin(), f[0].end(), 0 );
    }



    void testFordFulkerson()
    {
        cout << "FordFulkerson最大流" << endl;
        //生成P397页的图26-1(a)
        vector<string> v;
        v.push_back( "s" );
        v.push_back( "v1" );
        v.push_back( "v2" );
        v.push_back( "v3" );
        v.push_back( "v4" );
        v.push_back( "t" );
        GrpahicsViaAdjacencyMatrix<string, int> g( v, Digraph );
        g.Link2Vertex( 0, 1, 16 );
        g.Link2Vertex( 0, 2, 13 );
        g.Link2Vertex( 1, 3, 12 );
        g.Link2Vertex( 1, 2, 10 );
        g.Link2Vertex( 2, 1, 4 );
        g.Link2Vertex( 2, 4, 14 );
        g.Link2Vertex( 3, 2, 9 );
        g.Link2Vertex( 3, 5, 20 );
        g.Link2Vertex( 4, 3, 7 );
        g.Link2Vertex( 4, 5, 4 );

        cout << FordFulkerson( g ) << endl;
    }
}