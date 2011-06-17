//////////////////////////////////////////////////////////////////////////  
/// @file		disjoint_set_forest.h
/// @brief		用于不相交集合的数据结构（并查集）
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
/// 2011/06/17	1.0	谭川奇	创建

#include <iostream>
#include <vector>
using namespace std;

namespace ita
{

	/// @brief 采用树表示法实现的 用于不相交集合的数据结构（并查集）
	/// 
	/// 使用了“路径压缩”和“按秩合并”两种技术来加速并查集的运行时间。\n
	/// 这种方法实现的并查集的运行时间在实践上是线性的，但从理论上来说是超线性的。\n
	/// 用于不相交集合的数据结构又称为查并集：在很多的应用中（比如图的算法中经常使用，还有哈夫曼编码等），将N个不同的元素分成一组不相交的集合。\n
	/// 不相交集合上有两个重要的操作：<span style="color:#FF0000 ">找出给定元素所属的集合</span> 和 <span style="color:#FF0000 ">合并两个集合</span>。
	class DisjointSetForest
	{
	public:
		/// 集合中的一个元素
		template<typename T>
		struct DisjointSet
		{
			T				Item;		///< 元素的值
			int				Rank;		///< 元素的秩
			DisjointSet		*Parent;	///< 元素的父元素

			/// 创建一个结点：对应MAKE-SET操作
			DisjointSet(T const &item) : Item(item), Rank(0)
			{
				Parent = this;
			}
		};

		#pragma region 查并集的3个基本操作

		/// 创建一个集合的操作：MAKE-SET
		template<typename T>
		static DisjointSet<T> * MakeSet(T const &item)
		{
			return new DisjointSet<T>(item);
		}

		/// 查找所属集合的操作：FIND-SET
		/// @remarks	使用了路径压缩
		template<typename T>
		static DisjointSet<T> * FindSet(DisjointSet<T> *a_set)
		{
			//路径压缩
			if (a_set != a_set->Parent)		//判断本身不是代表
			{
				a_set->Parent = FindSet(a_set->Parent);
			}
			return a_set->Parent;
		}

		/// 合并操作：UNION
		/// @remarks	使用了按秩合并
		template<typename T>
		static void Union(DisjointSet<T> *x, DisjointSet<T> *y)
		{
			_Link(FindSet(x), FindSet(y));
		}

		#pragma endregion

	private:
		template<typename T>
		static void _Link(DisjointSet<T> *x, DisjointSet<T> *y)
		{
			if (x->Rank > y->Rank)
			{
				y->Parent = x;
			}
			else
			{
				x->Parent = y;
			}

			//按秩合并
			if (x->Rank == y->Rank)
			{//只有在秩相同时才会将最后的根结点的秩+1
				++(y->Rank);
			}
		}
	};
}