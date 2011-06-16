#include <iostream>
#include <vector>
using namespace std;

namespace ita
{

	/// 用于不相交集合的数据结构（并查集）
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

			if (x->Rank == y->Rank)
			{//只有在秩相同时才会将最后的根结点的秩+1
				++(y->Rank);
			}
		}
	};
}