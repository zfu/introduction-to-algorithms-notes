//////////////////////////////////////////////////////////////////////////
/// @file		best_binary_search_tree.cpp
/// @brief		最优二叉查找树
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
/// 2011/06/17   14:42	1.0	谭川奇	创建


#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <iomanip>
#include <limits>
#include <fstream>
using namespace std;

namespace ita
{
    /// @brief 最优二叉查找树
    ///
    /// 给定一个有序序列K={k1<k2<k3<,……,<kn}和他们被查询的概率P={p1,p2,p3,……,pn}，要求构造一棵二叉查找树T，使得查询所有元素的总的代价最小。
    /// 对于一个搜索树，当搜索的元素在树内时，表示搜索成功。当不在树内时，表示搜索失败，用一个“虚叶子节点”来标示搜索失败的情况，因此需要n+1个虚
    /// 叶子节点{d0<d1<……<dn}。其中d0表示搜索元素小于k1的失败结果，dn表示搜索元素大于kn的失败情况。di（0<i<n）表示搜索节点在ki和k(i+1)之间时
    /// 的失败情况。对于应di的概率序列是Q={q0,q1,……,qn}
    void BestBinarySearchTree()
    {
        //p1 ... p5：这里的p0是个占位符，为了后面的编码方便，p0是无意义的
        double p[] = {0, 0.15, 0.1, 0.05, 0.1, 0.2};
        double q[] = {0.05, 0.1, 0.05, 0.05, 0.05, 0.1};

        //e[i][j]表示从pi -- pj的最优二叉查找树的期望查找代价
        vector< vector<double> > e( 7, vector<double>( 6 ) );
        //w[i][j]表示从pi -- pj的概率总和
        vector< vector<double> > w( 7, vector<double>( 6 ) );

        //先计算出叶子结点单独成为一棵树时的期望查找代价
        //当e[i][j]的j<i时，e[i][j]代表的是叶子结点。e[i][i-1]代表的是第i-1个叶子结点的期望查找代价
        for ( int i = 1; i < 7; ++i )
        {
            e[i][i - 1] = q[i - 1];
        }

        //计算从pi -- pj的概率总和，包括普通结点和这里的虚拟叶子结点都要计算
        for ( int i = 1; i < 7; ++i )
        {
            for ( int j = i - 1; j < 6; ++j )
            {
                for ( int k = i; k <= j; ++k )
                {
                    //普通结点
                    w[i][j] += p[k];
                }
                for ( int k = i - 1; k <= j; ++k )
                {
                    //虚拟叶子结点
                    w[i][j] += q[k];
                }
            }
        }

        //这种按长度来进行递推的想法很明显，因为长度是依赖关系
        //欲知道长度为n的子集的最优二叉查找树，必定要先知道长度小于n的所有子集的最优二叉查找树
        for ( int length = 1; length <= 5; ++length )
        {
            //从长度短的子集开始推导长度长的子集
            for ( int i = 1; i < 6 - length + 1; ++i )
            {
                //对[i...j]这个子集进行计算它的最优期望查找代价
                int j = i + length - 1;

                vector<double> vs;
                for ( int k = i; k <= j; ++k )
                {
                    //i<=k<=j，从k位置将[i...j]进行划分，视pk为[pi...pj]这棵子树的根
                    vs.push_back( p[k] + e[i][k - 1] + w[i][k - 1] + e[k + 1][j] + w[k + 1][j] );
                }
                e[i][j] = *min_element( vs.begin(), vs.end() );
            }
        }

        //输出p1...p5的最优二叉查找树的期望查找代价
        cout << e[1][5] << endl;
        getchar();
    }
}