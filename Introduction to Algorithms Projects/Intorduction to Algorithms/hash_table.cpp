#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <iomanip>
#include <limits>
using namespace std;


namespace chapter11
{
	/// 全域散列表
	/// 
	/// 首先：全域散列表是一种使用“键接法”来解决碰撞问题的散列表方法。
	/// 全域散列函数类，首先选择一个足够大的质数p ，使得每一个可能的关键字k 都落到0 到p-1 的
	/// 范围内，包括首尾的0 和p-1。这里我们假设全域是0 – 15，p 为17。设集合Zp 为
	/// {0, 1, 2, …, p-1}，集合Zp* 为{1, 2, 3, …, p-1}。由于p 是质数，我们可以定义散列
	/// 函数h(a, b, k) = ((a*k + b) mod p) mod m。其中a 属于Zp，b 属于Zp*。
	/// 由所有这样的a 和b 构成的散列函数，组成了函数簇。即全域散列。
	/// 全域散列的基本思想是在执行<b>开始</b>时，从一族仔细设计的函数中，随机的选择一个作为散列函数。	
	/// 使之独立于要存储的关键字。不管对手选择了怎样的关键字，其平均性态都很好。
	template<typename T>		///< T是散列表里要存储的元素类型
	class UniversalHashTable
	{
	public:
		UniversalHashTable()
		{
			_p = 101;		//一个足够大的质数
			_m = 10;		//槽的个数
			_items.resize(_m, NULL);
			for (int i = 0; i < _m; ++i)
			{//全部先设置好头结点
				_items[i] = new _Node();
				_items[i]->Next = NULL;
			}

			// 全域散列的基本思想是在执行<b>开始</b>时，从一族仔细设计的函数中，随机的选择一个作为散列函数。
			_a = rand() % (_p - 1) + 1;
			_b = rand() % _p;			
		}

		~UniversalHashTable()
		{
			for_each(_items.begin(), _items.end(), [](_Node *item){
				while (item)
				{
					auto next = item->Next;
					delete item;
					item = next;
				}
			});
		}

		void Insert(T const &new_value)
		{//始终插入在键表的头，头结点之后的第1个位置
			_Node *new_item = new _Node;
			new_item->Item = new_value;
			new_item->Next = NULL;
			int hash_value = _HashFunction(new_value);

			new_item->Next = _items[hash_value]->Next;
			_items[hash_value]->Next = new_item;
		}

		void Delete(T const &delete_value)
		{
			int hash_value = _HashFunction(delete_value);
			_Node *root = _items[hash_value];

			while (root->Next)
			{
				if (root->Next->Item == delete_value)
				{
					auto temp = root->Next;
					root->Next = root->Next->Next;
					delete temp;
					break;
				}
				else
				{
					root = root->Next;
				}
			}
		}

		T * Search(T const &search_value)
		{
			int hash_value = _HashFunction(search_value);
			_Node *root = _items[hash_value]->Next;

			while (root)
			{
				if (root->Item == search_value)
				{
					return &(root->Item);
				}
				root = root->Next;
			}
			return NULL;
		}


		void Display()
		{
			for (int i = 0; i < _m; ++i)
			{
				_Node *item = _items[i]->Next;	//跳过头结点

				cout << "槽[" << setw(3) << i << setw(3) << "]";
				while (item)
				{
					cout << " -> " << item->Item;
					item = item->Next;
				}
				cout << endl;
			}
		}

	protected:

	private:
		/// 节点（使用单键表）
		/// 
		/// 要是用双键表就会方便很多啊
		struct _Node
		{
			T Item;
			_Node *Next;
		};

		/// 全域散列函数
		/// 
		/// 本函数、一开始时随机选取_a,_b、再加上选取_p,_m的方法，就是全域散列的核心所在！！！
		/// h(a, b, k) = ((a*k + b) mod p) mod m
		int _HashFunction(T k)
		{
			return static_cast<int>(_a * k + _b) % _p % _m;
		}


		int _p, _m, _a, _b;
		vector<_Node *> _items;		///< 存储真正的键表，使用的是带头结点的单键表
	};



	int test()
	{		
		UniversalHashTable<int> table;
		cout << "开始往UniversalHashTable里添加内容[0,100)：" << endl;
		for (int i = 0; i < 100; ++i)
		{
			table.Insert(i);
		}
		table.Display();

		cout << "开始删除内容[0,5)：" << endl;
		for (int i = 0; i < 5; ++i)
		{
			table.Delete(i);
		}
		table.Display();
		
		for (int i = 0; i < 10; ++i)
		{
			auto finded = table.Search(i);
			cout << "开始检索结点[" << i << "]：";
			if (finded)
			{
				cout << *finded << endl;
			}
			else
			{
				cout <<  "未找到" << endl;
			}			
		}

		return 0;
	}

}