//////////////////////////////////////////////////////////////////////////
/// @file		quick_sort.cpp
/// @brief		快速排序
/// @details	COPYRIGHT NOTICE
///			    Copyright (c) 2011
///			    All rights reserved.\n
///			    实现了快速排序算法以及《算法导论》7-6题的“对模糊区间的快速排序
///
/// @author		谭川奇	chuanqi.tan(at)gmail.com
/// @date		2011/06/17
/// @version	1.0
//////////////////////////////////////////////////////////////////////////
/// 修改记录：
/// 2011/06/17   10:30	1.0	谭川奇	创建


#include <vector>
#include <iostream>
#include <iterator>
#include <ctime>
#include <algorithm>
using namespace std;

namespace ita
{

    /// @brief 采用了随机取样技术的快速排序
    ///
    /// 快速排序的平均效率为O(nlgn)，最坏情况为O(n^2)
    void QuickSort( vector<int> &ToSort, int BeginIndex, int EndIndex )
    {
        if( BeginIndex < EndIndex )
        {
            //加入随机取样的随机化技术
            //一定要使用，对平均性能的提升作用太大了
            int random_swap = ( rand() % ( EndIndex - BeginIndex + 1 ) ) + BeginIndex;
            std::swap( ToSort[random_swap], ToSort[EndIndex] );

            //i代表的是比ToSort[EndIndex]小的元素的上界，即ToSort[0...i)的元素值都比ToSort[EndIndex]要小
            //也意味着下一个比ToSort[EndIndex]小的元素要放置的位置；但是在当前可能ToSort[i] >= ToSort[EndIndex]
            int i = BeginIndex;

            //j代表已经检查过的元素的上界
            for ( int j = BeginIndex; j != EndIndex; ++j )
            {
                //找到满足比ToSort[EndIndex]小的元素
                if ( ToSort[j] < ToSort[EndIndex] )
                {
                    swap( ToSort[i], ToSort[j] );	//将这个比ToSort[EndIndex]小的元素移到第i个去,满足了i代表的意义
                    ++i;						//由于新找到了一个比ToSort[EndIndex]小的元素,所以上界应该+1
                }
            }
            swap( ToSort[i], ToSort[EndIndex] );

            QuickSort( ToSort, BeginIndex, i - 1 );
            QuickSort( ToSort, i + 1, EndIndex );
        }
    }


    /// @brief 对模糊区间的快速排序
    ///
    /// 问题描述： （算法导论7-6题）\n
    /// 考虑这样的一种排序问题，即无法准确地知道待排序的各个数字到底是多少。对于其中的每个数字，
    /// 我们只知道它落在实轴上的某个区间内。亦即，给定的是n个形如[a(i), b(i)]的闭区间（这里小括
    /// 后起下标的作用，后同），其中a(i) <= b(i)。算法的目标是对这些区间进行模糊排序
    /// （fuzzy-sort），亦即，产生各区间的一个排列<i(1), i(2), ..., i(n)>，使得存在一个c(j)属于
    /// 区间[a(i(j)), b(i(j))]，满足c(1) <= c(2) <= c(3) <= ... <= c(n)。 :\n
    /// -   为n个区间的模糊排序设计一个算法。你的算法应该具有算法的一般结构，它可以快速排序左部
    ///		端点（即各a(i)），也要能充分利用重叠区间来改善运行时间。（随着各区间重叠得越来越多，
    ///		对各区间进行模糊排序的问题会变得越来越容易。你的算法应能充分利用这种重叠。） \n
    /// -   证明：在一般情况下，你的算法的期望运行时间为Θ(nlgn)，但当所有的区间都重叠时，期望的
    ///		运行时间为Θ(n)（亦即，当存在一个值x，使得对所有的i，都有x∈[a(i), b(i)]）。你的算法
    ///		不应显式地检查这种情况，而是应随着重叠量的增加，性能自然地有所改善。
    void SmoothQuickSort( vector< pair<int, int> > &to_sort, int begin_index, int end_index )
    {
        if ( begin_index < end_index )
        {
            //取最后一个区间为主元
            auto last = to_sort[end_index];
            //获取要比较的区间（除去主元）为[begin_index, end_index) => [i,j]
            //区间[i,j]意思是：其中所有的元素要不还未处理，要不相互重叠有至少一个重叠值，并且该值还与to_sort[end_index]重叠
            //即在题目中规定的语义下与to_sort[end_index]绝对相等
            int i = begin_index;
            int j = end_index - 1;

            for ( int k = begin_index; k <= j; )
            {
                if ( to_sort[k].second <= last.first )
                {
                    //严格小于主元
                    swap( to_sort[i], to_sort[k] );
                    ++i;
                    ++k;
                }
                else if ( to_sort[k].first >= last.second )
                {
                    //严格大于主元
                    swap( to_sort[j], to_sort[k] );
                    --j;
                }
                else
                {
                    // 与主元区间有重叠，则更新主元为重叠区间（交集）
                    // 此方法参考了 http://blogold.chinaunix.net/u/18517/showart_487873.html
                    //
                    // 这种想法真的好，因为缩小了主元的区间（交集），所以就可以认为以后任何与缩小之后主元有重叠的区间都一定与当前区间to_sort[k]重叠（因为它完全包括缩小后的主元）\n
                    // 因此这样就可以确定最后在[i,j]中的所有元素在本题的约定下与主元绝对相等（即所有的元素相互重叠），所以不需要再处理。这就符合了题目中的“充分利用重叠区间来改善运行时间”\n
                    // 如果没有这步缩小区间，就只能认为[i,j]中的元素各自与主元有重叠而无法判断为绝对相等。
                    // @note	这里很容易弄错的一点是：区间重叠并没有传递性，重叠区间的元素并不能认为是已序的
                    last.first = max( to_sort[k].first, last.first );
                    last.second = min( to_sort[k].second, last.second );
                    ++k;
                }
            }
            swap( to_sort[j + 1], to_sort[end_index] );

            SmoothQuickSort( to_sort, begin_index, i - 1 );
            SmoothQuickSort( to_sort, j + 2, end_index );
        }
    }

    /// 测试快速排序和对模糊区间的快速排序
    int testQuickSort()
    {
        cout << "==========================快速排序=============================" << endl;
        vector<int> ToSort;
        for ( int i = 0; i < 100; ++i )
        {
            ToSort.push_back( rand() );
        }
        cout << "随机填充100个数：" << endl;
        copy( ToSort.begin(), ToSort.end(), ostream_iterator<int>( cout, "  " ) );
        QuickSort( ToSort, 0, ToSort.size() - 1 );
        cout << endl << "快速排序的结果如下：" << endl;
        copy( ToSort.begin(), ToSort.end(), ostream_iterator<int>( cout, "  " ) );

        cout << endl << "======================模糊区间的快速排序=========================" << endl;
        vector< pair<int, int> > to_sort_smooth;
        for ( int i = 0; i < 10; ++i )
        {
            int b = rand() % 100;
            to_sort_smooth.push_back( make_pair( b, b + rand() % 100 ) );
        }
        SmoothQuickSort( to_sort_smooth, 0, to_sort_smooth.size() - 1 );
        for_each( to_sort_smooth.begin(), to_sort_smooth.end(), []( pair<int, int> &p )
        {
            cout << p.first << "\t --> \t" << p.second << endl;
        } );

        return 0;
    }

}