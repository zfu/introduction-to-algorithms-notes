//////////////////////////////////////////////////////////////////////////
/// @file		priority_queue.h
/// @brief		优先队列
/// @details	COPYRIGHT NOTICE
///			    Copyright (c) 2011
///			    All rights reserved.\n
///			    实现了一个优先队列
///
/// @author		谭川奇	chuanqi.tan(at)gmail.com
/// @date		2011-5-25
/// @version	1.0
//////////////////////////////////////////////////////////////////////////
/// 修改记录：
/// 2010/6/17	1.0	谭川奇	创建

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <queue>
using namespace std;

namespace ita
{
	/// @brief 优先队列
	///
	/// 堆的主要应用之一：优先队列。\n
	/// 优先级最高的元素在队首，其它的元素依赖于“比较子”满足大头堆的性质。
	/// @param	ItemType		队列中元素的类型
	/// @param	Comparator		用于比较队列中元素优先级的比较子
	/// @param	ContainerType	优先队列内部所使用的容器类型
	/// @see	void MakeHeap(vector<int> &, size_t , size_t)
	/// @see	void BuildHeap(vector<int> &)
	/// @note	优先列队中的元素一旦被加入到队列中去了就不应该再修改；对于队列中的元素应该只支持Top, Pop, Push操作
	template <
		typename ItemType,
		typename Comparator = less<ItemType>,
		typename ContainerType = vector<ItemType >>
	class PriorityQueue
	{
	public:
		typedef PriorityQueue<ItemType, Comparator, ContainerType>	_MyType;
		typedef	ItemType &			Reference;
		typedef ItemType const &	ConstReference;

		/// 创建一个空的优先队列
		PriorityQueue()
		{}

		/// 由一个区间初始化一个优先队列
		template<typename IterType>
		PriorityQueue( IterType begin, IterType end ) : _queue( begin, end )
		{
			make_heap( _queue.begin(), _queue.end(), _comparator );
		}

		/// 向队列中添加一个元素
		void Push( ItemType const &item )
		{
			_queue.push_back( item );
			push_heap( _queue.begin(), _queue.end(), _comparator );
		}

		/// @brief 访问const的顶端元素
		/// @return		优先级最高的队首元素的const引用
		ConstReference Top() const
		{
			return *_queue.begin();
		}

		/// @brief 访问非const的顶端元素
		///
		/// 在标准STL的优先队列实现中有这个非const方法，但是我认为不应该有这个方法，因为这样的话可以通过这个非const引用来修改Top Item的优先级，这样整个列队就可能不再具有一致性了！\n
		/// 如果需要改变优先级，应该使用RefreshQueue里介绍的方法
		/// @return		优先级最高的队首元素的<b>非const</b>引用
		/// @see	void RefreshQueue()
		/// @deprecated		<b>如果修改了队首元素的优先级，可能引起优先队列内部的不一致性</b>
		Reference Top()
		{
			return *_queue.begin();
		}

		/// 队首的元素出队
		void Pop()
		{
			pop_heap( _queue.begin(), _queue.end(), _comparator );
			_queue.pop_back();
		}

		/// 查询队列是否为空
		bool IsEmpty()
		{
			return _queue.empty();
		}

		/// @brief 重新排序优先列队中的元素
		///
		/// 这个方法非常重要，有了这个方法之后优先列队就可以支持修改优先级的操作了。
		/// @code
		/// auto comparator = [](ItemType *item1, ItemType *item2){return item1->Priority() < item2->Priority();};
		/// PriorityQueue<ItemType *, comparator> q;
		/// items[6]->SetPriority(66);		//从别处改变了某个元素的优先级
		/// q.RefreshQueue();				//一定要记得调用RefreshQueue函数，否则在元素的优先级被外部修改之后优先队列的内部状态将不一致！		///
		/// @endcode
		void RefreshQueue()
		{
			make_heap( _queue.begin(), _queue.end(), _comparator );
		}

		/// 将队列中所有的元素显示的输出流中
		void Display()
		{
			copy( _queue.begin(), _queue.end(), ostream_iterator<ItemType>( cout, "  " ) );
			cout << endl;
		}

	private:
		ContainerType			_queue;			///< 容器
		Comparator				_comparator;	///< 比较子
	};

}