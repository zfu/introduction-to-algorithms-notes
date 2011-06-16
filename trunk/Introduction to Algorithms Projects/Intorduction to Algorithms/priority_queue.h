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



#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <queue>
using namespace std;

namespace ita
{
	/// 优先队列
	/// 
	/// 堆的主要应用之一：优先队列
	/// @notes	优先列队中的元素一旦被加入到队列中去了就不应该再修改；对于队列中的元素应该只支持Top, Pop, Push操作
	template<
		typename ItemType,								///< 队列中元素类型
		typename Comparator = less<ItemType>,			///< 判断优先级时所使用的断言
		typename ContainerType = vector<ItemType>>		///< 队列内部所使用的容器类型
	class PriorityQueue
	{
	public:
		typedef PriorityQueue<ItemType, Comparator, ContainerType>	_MyType;
		typedef	ItemType &			Reference;
		typedef ItemType const &	ConstReference;

		PriorityQueue()
		{}

		template<typename IterType>
		PriorityQueue(IterType begin, IterType end) : _queue(begin, end)
		{
			make_heap(_queue.begin(), _queue.end(), _comparator);
		}

		void Push(ItemType const &item)
		{
			_queue.push_back(item);
			push_heap(_queue.begin(), _queue.end(), _comparator);
		}

		ConstReference Top() const
		{
			return *_queue.begin();
		}

		///// 返回非const的顶端元素
		///// 
		///// 我认为不应该有这个方法，因为这样的话可以通过这个非const引用来修改Top Item的优先级，这样整个列队就可能不再具有一致性了！
		///// 如果需要改变优先级，应该使用RefreshQueue里介绍的方法
		///// @deprecated
		//Reference Top()
		//{
		//	return *_queue.begin();
		//}

		void Pop()
		{
			pop_heap(_queue.begin(), _queue.end(), _comparator);
			_queue.pop_back();
		}

		bool IsEmpty()
		{
			return _queue.empty();
		}

		/// 重新排序优先列队中的元素
		/// 
		/// 这个方法非常重要，有了这个方法之后优先列队就可以支持修改优先级的操作了。
		/// @code
		/// 
		/// auto comparator = [](ItemType *item1, ItemType *item2){return item1->Priority() < item2->Priority();};
		/// PriorityQueue<ItemType *, comparator> q;
		/// items[6]->SetPriority(66);		//从别处改变了某个元素的优先级
		/// q.RefreshQueue();				//一定要记得调用RefreshQueue函数，否则在元素的优先级被外部修改之后优先队列的内部状态将不一致！
		/// 
		/// @endcode
		void RefreshQueue()
		{
			make_heap(_queue.begin(), _queue.end(), _comparator);
		}

		void Display()
		{
			copy(_queue.begin(), _queue.end(), ostream_iterator<ItemType>(cout, "  "));
			cout << endl;
		}

	private:
		ContainerType			_queue;			///< 容器
		Comparator				_comparator;	///< 比较子
	};

}