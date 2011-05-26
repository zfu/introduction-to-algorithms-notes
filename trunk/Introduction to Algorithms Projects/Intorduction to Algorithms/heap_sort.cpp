//////////////////////////////////////////////////////////////////////////  
///    COPYRIGHT NOTICE  
///    Copyright (c) 2009
///    All rights reserved.  
///  
/// @file		HeapSort.cpp  
/// @brief		堆排序的学习，使用大头堆  
///  
///  
/// @author		谭川奇	chuanqi.tan@gmail.com
/// @date		2011-5-25
///  
///  
///    修订说明：最初版本  
//////////////////////////////////////////////////////////////////////////  

#include "stdafx.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

namespace chapter6
{


/// 保持堆的性质
/// 
/// 将to_make的[0,length)元素视为一棵完全二叉树，以第i个元素为根的子树除了第i个元素之外都满足大堆的性质
/// 调用此方法之后，这棵完全二叉树以第i个元素为根的子树都满足大堆的性质
/// @to_make	保存数据的数组
/// @length		标记to_make的[0,length)元素视为一个完全二叉树
///				第length个元素之后[length, n)的元素不包括在这棵完全二叉树里
/// @i			需要处理的第i个元素
/// @note		to_make的前length个元素并不一定是一个堆（因为它不满足大堆的性质），但可以映射为完全二叉树
void MakeHeap(vector<int> &to_make, size_t length, size_t i)
{
	size_t left = 2 * i + 1;
	size_t right = 2 * i + 2;
	size_t the_max = i;

	if (left < length && to_make[left] > to_make[i])
	{
		the_max = left;
	}	
	if (right < length && to_make[right] > to_make[the_max])
	{
		the_max = right;
	}

	if (the_max != i)
	{
		std::swap(to_make[i], to_make[the_max]);
		MakeHeap(to_make, length, the_max);
	}
}

/// 建堆
/// 
/// 将to_built数组改建成一个大头堆
void BuildHeap(vector<int> &to_built)
{
	//这里只需要从to_built.size() / 2 - 1开始的原因在于：
	//对叶子结点来说，它和它的子结点（为空）总是满足堆的定义的，所以只需要处理非叶子结点
	for (int i = to_built.size() / 2 - 1; i >= 0; --i)
	{
		MakeHeap(to_built, to_built.size(), i);
	}
}

/// 堆排序
void HeapSort(vector<int> &to_sort)
{
	BuildHeap(to_sort);
	for (int i = to_sort.size() - 1; i > 0; --i)
	{
		std::swap(to_sort[0], to_sort[i]);
		MakeHeap(to_sort, i, 0);
	}
}


/// 优先队列
/// 
/// 堆的主要应用之一：优先队列
class PriorityQueue
{
public:
	PriorityQueue()
	{
		int to_init[] = {8,5,78,45,64,987,45,34,23,4,23};
		_queue.assign(to_init, to_init + sizeof(to_init) / sizeof(int));
		BuildHeap(_queue);
	}

	int GetMaxPriority()
	{
		return _queue[0];
	}

	int GetAndPopMaxPriority()
	{
		int max_priority = GetMaxPriority();

		std::swap(_queue[0], _queue[_queue.size() - 1]);
		_queue.erase(_queue.end() - 1);
		MakeHeap(_queue, _queue.size(), 0);

		return max_priority;
	}

	void ChangePriority(int index, int new_priority)
	{
		if (_queue[index] == new_priority)
		{//优先级不变，直接返回
			return;
		}

		if (_queue[index] > new_priority)
		{//降底优先级
			_queue[index] = new_priority;
			MakeHeap(_queue, _queue.size(), index);
		}
		else
		{//提升优先级			
			_queue[index] = new_priority;
			while(index > 0)
			{
				int parent_index = static_cast<int>((index + 1) / 2) - 1;

				if (_queue[parent_index] < _queue[index])
				{
					std::swap(_queue[parent_index], _queue[index]);
					index = parent_index;
				}
				else
				{
					break;
				}			
			}
		}
	}

	void Insert(int priority)
	{
		_queue.push_back(std::numeric_limits<int>::min());
		ChangePriority(_queue.size() - 1, priority);
	}

	bool IsEmpty()
	{
		return _queue.empty();
	}

	void Display()
	{
		copy(_queue.begin(), _queue.end(), ostream_iterator<int>(cout, "  "));
		cout << endl;
	}

private:
	vector<int> _queue;
};



int test()
{
	int to_init[] = {8,5,78,45,64,987,45,34,23,4,23};
	vector<int> to_sort(to_init, to_init + sizeof(to_init) / sizeof(int));
	cout << "原始数组，准备进行堆排序：";
	copy(to_sort.begin(), to_sort.end(), ostream_iterator<int>(cout, "  "));
	HeapSort(to_sort);
	//reverse(result.begin(), result.end());
	cout << endl << "堆排序结束：";
	copy(to_sort.begin(), to_sort.end(), ostream_iterator<int>(cout, "  "));
	cout << endl;

	cout << "初始化一个优先队列：";
	PriorityQueue queue;
	queue.Display();
	cout << "开始不断的取最高优先级的任务出列：" << endl;
	while(!queue.IsEmpty())
	{
		cout << queue.GetAndPopMaxPriority() << ":\t";
		queue.Display();
	}

	cout << "开始添加任务入列：" << endl;
	for (size_t i = 0; i < to_sort.size(); ++i)
	{
		queue.Insert(to_sort[i]);
		queue.Display();
	}

	cout << "更改一个任务的优先级：任务2优先级调整为-9并自动调整堆：" << endl;
	queue.ChangePriority(2, -9);
	queue.Display();

	return 0;
}

}