//////////////////////////////////////////////////////////////////////////
/// @file		heap_sort.cpp
/// @brief		堆的使用与堆排序
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
/// 2011/06/17   10:18	1.0	谭川奇	创建


#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "priority_queue.h"
using namespace std;

namespace ita
{


	/// @brief 保持堆的性质
	///
	/// 将to_make的[0,length)元素视为一棵完全二叉树，以第i个元素为根的子树除了第i个元素之外都满足大堆的性质
	/// 调用此方法之后，这棵完全二叉树以第i个元素为根的子树都满足大堆的性质
	/// @param	to_make		保存数据的数组
	/// @param	length		标记to_make的[0,length)元素视为一个完全二叉树<br/>
	///						第length个元素之后[length, n)的元素不包括在这棵完全二叉树里
	/// @param	i			需要处理的第i个元素
	/// @note				to_make的前length个元素并不一定是一个堆（因为它不满足大堆的性质），但可以映射为完全二叉树
	void MakeHeap( vector<int> &to_make, size_t length, size_t i )
	{
		size_t left = 2 * i + 1;
		size_t right = 2 * i + 2;
		size_t the_max = i;

		if ( left < length && to_make[left] > to_make[i] )
		{
			the_max = left;
		}
		if ( right < length && to_make[right] > to_make[the_max] )
		{
			the_max = right;
		}

		if ( the_max != i )
		{
			std::swap( to_make[i], to_make[the_max] );
			MakeHeap( to_make, length, the_max );
		}
	}

	/// @brief 建堆
	///
	/// 将to_built数组改建成一个大头堆
	void BuildHeap( vector<int> &to_built )
	{
		//这里只需要从to_built.size() / 2 - 1开始的原因在于：
		//对叶子结点来说，它和它的子结点（为空）总是满足堆的定义的，所以只需要处理非叶子结点
		for ( int i = to_built.size() / 2 - 1; i >= 0; --i )
		{
			MakeHeap( to_built, to_built.size(), i );
		}
	}

	/// 堆排序
	void HeapSort( vector<int> &to_sort )
	{
		BuildHeap( to_sort );
		for ( int i = to_sort.size() - 1; i > 0; --i )
		{
			std::swap( to_sort[0], to_sort[i] );
			MakeHeap( to_sort, i, 0 );
		}
	}

	/// 测试 堆排序与优先队列 的实现
	int testHeapSort()
	{
		int to_init[] = {8, 5, 78, 45, 64, 987, 45, 34, 23, 4, 23};
		vector<int> to_sort( to_init, to_init + sizeof( to_init ) / sizeof( int ) );
		cout << "原始数组，准备进行堆排序：";
		copy( to_sort.begin(), to_sort.end(), ostream_iterator<int>( cout, "  " ) );
		HeapSort( to_sort );
		cout << endl << "堆排序结束：";
		copy( to_sort.begin(), to_sort.end(), ostream_iterator<int>( cout, "  " ) );
		cout << endl << endl;

		cout << "初始化一个优先队列：";
		PriorityQueue<int, greater<int>> queue;
		for ( int i = 0; i < 10; ++i )
		{
			queue.Push( rand() % 1000 );
		}
		queue.Display();

		cout << "开始不断的取最高优先级的任务出列：" << endl;
		while( !queue.IsEmpty() )
		{
			cout << queue.Top() << ":\t";
			queue.Pop();
			queue.Display();
		}

		cout << "开始添加任务入列：" << endl;
		for ( size_t i = 0; i < to_sort.size(); ++i )
		{
			queue.Push( to_sort[i] );
			queue.Display();
		}

		queue.Display();

		return 0;
	}

}