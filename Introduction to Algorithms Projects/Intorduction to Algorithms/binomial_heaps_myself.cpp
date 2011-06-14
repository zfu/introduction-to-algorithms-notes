#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include "graphviz_shower.h"
using namespace std;

namespace chapter19
{
	/// 二项堆
	template<typename KeyType>
	class BinomialHeaps
	{
	public:
		struct BinomialHeapNode
		{
			BinomialHeapNode	*Parent;
			BinomialHeapNode	*Child;
			BinomialHeapNode	*Sibling;		

			unsigned int 		Degree;
			KeyType				Key;
		};

		/// 建立一个空的二项堆
		BinomialHeaps()
		{
			_head_list = nullptr;
		}

		~BinomialHeaps()
		{
			//TODO:delete all nodes
		}

		/// 在二项堆中插入一个新结点
		void Insert(KeyType new_key)
		{
			BinomialHeaps new_heap;
			new_heap._head_list = new BinomialHeapNode();
			new_heap._head_list->Parent = new_heap._head_list->Child = new_heap._head_list->Sibling = nullptr;
			new_heap._head_list->Degree = 0;
			new_heap._head_list->Key = new_key;

			this->Union(new_heap);
		}

		KeyType Mininum() const
		{
			vector<KeyType> values_in_head_list;
			BinomialHeapNode *node = _head_list;
			while(node != nullptr)
			{
				values_in_head_list.push_back(node->Key);
				node = node->Sibling;
			}
			return *min_element(values_in_head_list.begin(), values_in_head_list.end());
		}

		KeyType ExtractMinium()
		{
			vector<BinomialHeapNode *> head_nodes;
			BinomialHeapNode *l = _head_list;
			while(l)
			{
				head_nodes.push_back(l);
				l = l->Sibling;
			}
			auto min_ele = min_element(head_nodes.begin(), head_nodes.end(), [](BinomialHeapNode *left, BinomialHeapNode *right){
				return left->Key < right->Key;
			});
			int min_index = min_ele - head_nodes.begin();
			KeyType min_value = (*min_ele)->Key;
			BinomialHeapNode	*min_node = head_nodes[min_index];

			if (min_index == 0)
			{
				if (head_nodes.size() > 1)
				{
					_head_list = head_nodes[1];
				}
				else
				{
					//根链上只有一个元素
					_head_list = nullptr;
				}
			}
			else if (min_index == head_nodes.size() - 1)
			{
				head_nodes[min_index - 1]->Sibling = nullptr;
			}
			else
			{
				head_nodes[min_index - 1]->Sibling = head_nodes[min_index + 1];
			}

			BinomialHeaps new_head;
			new_head._head_list = min_node->Child;
			BinomialHeapNode *x = new_head._head_list;
			while (x)
			{
				x->Parent = nullptr;
				x = x->Sibling;
			}
			this->Union(new_head);
			
			delete min_node;
			return min_value;
		}

		void Decrease(BinomialHeapNode *x, KeyType k)
		{
			if (k > x->Key)
			{
				throw exception("只能减小不能增大");
			}

			x->Key = k;
			BinomialHeapNode *y = x;
			BinomialHeapNode *z = y->Parent;
			while (z && y->Key < z->Key)
			{
				swap(y->Key, z->Key);
				y = z;
				z = y->Parent;
			}
		}

		void Delete(BinomialHeapNode *node)
		{
			Decrease(node, numeric_limits<KeyType>::min());
			ExtractMinium();
		}
		
		/// 联合另外一个二项堆
		/// 
		/// 将另外一个二项堆联合到本二项堆,当联合操作完成之后,other的二项堆中的数据将无效
		void Union(BinomialHeaps &other)
		{
			vector<BinomialHeapNode *> nodes;
			BinomialHeapNode *l = _head_list;
			BinomialHeapNode *r = other._head_list;			
			while (l)
			{
				nodes.push_back(l);
				l = l->Sibling;
			}
			while (r)
			{
				nodes.push_back(r);
				r = r->Sibling;
			}
			if (nodes.empty())
			{
				return;
			}

			// sort and merge
			sort(nodes.begin(), nodes.end(), [](BinomialHeapNode *left, BinomialHeapNode *right){
				return left->Degree < right->Degree;
			});
			for (size_t i = 0; i < nodes.size() - 1; ++i)
			{
				nodes[i]->Sibling = nodes[i + 1];
			}
			nodes[nodes.size() - 1]->Sibling = nullptr;
			
			// reset head list
			this->_head_list = nodes[0];
			other._head_list = nullptr;		//destory binomial heaps has been unioned
			if (this->_head_list == nullptr)
			{
				return;
			}

			// begin to merge nodes in head list with same degree
			BinomialHeapNode *prev_x = nullptr;
			BinomialHeapNode *x = _head_list;
			BinomialHeapNode *next_x = x->Sibling;

			while(next_x)
			{
				if (x->Degree != next_x->Degree || (next_x->Sibling != nullptr && next_x->Sibling->Degree == x->Degree))
				{
					prev_x = x;
					x = next_x;
				}
				else if (x->Key < next_x->Key)
				{
					x->Sibling = next_x->Sibling;
					_Link(next_x, x);
				}
				else
				{
					if (prev_x == nullptr)
					{
						_head_list = next_x;
					}
					else
					{
						prev_x->Sibling = next_x;
					}

					_Link(x, next_x);
					x = next_x;
				}

				next_x = x->Sibling;
			}
		}
	
		BinomialHeapNode * Search(KeyType key) const
		{
			BinomialHeapNode *tree = _head_list;

			//遍历根链
			while(tree)
			{
				BinomialHeapNode *node = _SearchInTree(tree, key);
				if (node != nullptr)
				{
					return node;
				}
				tree = tree->Sibling;
			}
			return nullptr;
		}

		bool IsEmpty() const
		{
			return _head_list == nullptr;
		}

		BinomialHeapNode const * const GetHeadList() const
		{
			return _head_list;
		}

		void Display() const
		{
			stringstream ss;

			ss << "digraph graphname" << "{" << endl
				<< "    RootList [shape = box];" << endl;

			BinomialHeapNode *node = _head_list;
			if (node)
			{
				ss << "    RootList -> " << node->Key << ";" << endl;
			}			
			while(node)
			{
				_Display(ss, node);

				if (node->Sibling)
				{
					ss << "    " << node->Key << " -> " << node->Sibling->Key << ";" << endl;
				}
				node = node->Sibling;
			}

			ss << "}" << endl;
			qi::ShowGraphvizViaDot(ss.str());

		}

	private:
		void _Link(BinomialHeapNode *y, BinomialHeapNode *z)
		{
			y->Parent = z;
			y->Sibling = z->Child;
			z->Child = y;
			++z->Degree;
		}
		BinomialHeapNode * _SearchInTree(BinomialHeapNode *tree, KeyType key) const
		{
			if (tree->Key == key)
			{
				return tree;
			}

			BinomialHeapNode *node = tree->Child;
			while(node)
			{
				BinomialHeapNode *n = _SearchInTree(node, key);
				if (n)
				{
					return n;
				}
				node = node->Sibling;
			}

			return nullptr;
		}

		//画一棵二项树
		void _Display(stringstream &ss, BinomialHeapNode *tree) const
		{
			if (tree)
			{
				BinomialHeapNode *child = tree->Child;
				if (child)
				{
					vector<BinomialHeapNode *> childs;
					while (child)
					{
						childs.push_back(child);
						child = child->Sibling;
					}

					for_each(childs.begin(), childs.end(), [&](BinomialHeapNode *c){
						ss << "    " << c->Key << " -> " << tree->Key << ";" << endl;
						_Display(ss, c);
					});
				}
			}
		}

	private:
		BinomialHeapNode *_head_list;			//根表
	};


	void testBinomialHeaps()
	{
		cout << "二项堆" << endl;
		BinomialHeaps<int> bh;
		for (int i = 0; i < 10; ++i)
		{
			bh.Insert(rand() % 100);
		}
		bh.Display();

		while (!bh.IsEmpty())
		{
			cout << bh.ExtractMinium() << "\t";
		}
		bh.Display();

		for (int i = 0; i < 64; ++i)
		{
			int r = rand() % 100;
			if (bh.Search(r) == nullptr)
			{
				bh.Insert(r);
			}			
		}
		bh.Display();

		bh.Delete(const_cast<BinomialHeaps<int>::BinomialHeapNode *>(bh.GetHeadList()));
		bh.Display();
	}
}