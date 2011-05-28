#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <iomanip>
#include <limits>
using namespace std;

namespace chapter12
{
	/// 二叉查找树
	class BinarySearchTree
	{
	private:
		/// 二叉查找树中的结点
		struct _Node
		{
			int Item;
			_Node *Parent;
			_Node *Left;
			_Node *Right;
		};

	public:
		BinarySearchTree() : _root(NULL)
		{}

		~BinarySearchTree()
		{
			//从根结点开始递归的析构
			_RecursiveReleaseNode(_root);
		}

		/// 插入一个结点
		/// 
		/// 向二叉查找树中插入一个值
		/// @param		要插入的值
		/// @param		是否插入成功，失败意味着树中已经存在该值
		bool Insert(int const new_value)
		{
			if (Search(new_value))
			{//已经存在
				return false;
			}

			if (!_root)
			{//插入的是第1个节点
				_root = new _Node();
				_root->Item = new_value;
				return true;
			}

			//非第1个节点
			_Node *current_node = _root;

			while (current_node)
			{
				if (new_value > current_node->Item)
				{
					if (current_node->Right)
					{
						current_node = current_node->Right;
					}
					else
					{
						current_node->Right = new _Node();
						current_node->Right->Item = new_value;
						current_node->Right->Parent = current_node;
						break;
					}
				}
				if (new_value < current_node->Item)
				{
					if (current_node->Left)
					{
						current_node = current_node->Left;
					}
					else
					{
						current_node->Left = new _Node();
						current_node->Left->Item = new_value;
						current_node->Left->Parent = current_node;
						break;
					}
				}
			}
		}

		/// 删除结点
		/// 
		/// 在二叉查找树中删除一个值
		/// @param		要删除的值
		/// @return		是否删除成功，删除失败意味着树中没有这个值的结点
		bool Delete(int const delete_value)
		{
			_Node *delete_node = _Search(_root, delete_value);
			if (!delete_node)
			{//未找到该点
				return false;
			}
			else
			{
				_DeleteNode(delete_node);
				return true;
			}
		}

		/// 查找元素
		/// 
		/// 在当前二叉查找树中查找某一值
		/// @param		search_value要查找的值
		/// @return		是否在二叉树中找到值为search_value的结点
		bool Search(int const search_value) const
		{
			return _Search(_root, search_value);
		}

		/// 显示当前二叉查找树的状态
		/// 
		/// 使用Graphviz的dot语言画出当前二叉查找树
		void Display() const
		{
			cout << "digraph graphname" << (rand() % 1000) << "{" << endl
				 << "    node [shape = record,height = .1];" << endl;
			_Display(_root);
			cout << "}" << endl;
		}
	
	private:
		void _DeleteNode( _Node * delete_node ) 
		{
			if (delete_node->Left && delete_node->Right)
			{//要删除的结点同时存在左子树和右子树
				//前驱结点：前驱一定存在，因为该结点同时存在左右子树
				_Node *previous_node = delete_node; 
				//求结点的前驱：先左一下，再右到头
				previous_node = previous_node->Left;
				while (previous_node->Right)
				{
					previous_node = previous_node->Right;
				}

				delete_node->Item = previous_node->Item;
				//previous_nde一定没有右子树，所以再递归调用一定是走这个if的else分支
				_DeleteNode(previous_node);	
			}
			else
			{//要删除的结点至少有一个子结点为空
				//sub_node为delete_node的子树
				//sub_node要么为delete_node的左子树，要么为delete_node的右子树，或者在delete_node无子结点时为空
				_Node *sub_node = (delete_node->Left ? delete_node->Left : delete_node->Right);

				if (delete_node->Parent == NULL)
				{//是第1个结点
					_root = sub_node;
				}
				else
				{
					if (delete_node->Parent->Left == delete_node)
					{
						delete_node->Parent->Left = sub_node;
					}
					else
					{
						delete_node->Parent->Right = sub_node;
					}

					if (sub_node)
					{//在delete_node有子结点时需要设置子结点的Parent指针
						sub_node->Parent = delete_node->Parent;
					}
				}

				delete delete_node;
			}
		}

		void _RecursiveReleaseNode(_Node *node)
		{
			if (node)
			{			
				_RecursiveReleaseNode(node->Left);
				_RecursiveReleaseNode(node->Right);
				delete node;
			}
		}

		_Node * _Search(_Node *node, int const search_value) const
		{
			if (node)
			{
				if (node->Item == search_value)
				{
					return node;
				}
				else if (search_value < node->Item)
				{
					return _Search(node->Left, search_value);
				}
				else
				{
					return _Search(node->Right, search_value);
				}
			}
			else
			{//node 为空当然是未找到
				return NULL;
			}
		}

		void _Display(_Node *node) const
		{
			if (node)
			{
				cout << "    node" << node->Item << "[label = \"<f0>|<f1>" << node->Item << "|<f2>\"];" << endl;

				if (node->Left)
				{
					cout << "    \"node" << node->Item << "\":f0 -> \"node" << node->Left->Item << "\":f1;" << endl;
					_Display(node->Left);
				}
				
				if (node->Right)
				{
					cout << "    \"node" << node->Item << "\":f2 -> \"node" << node->Right->Item << "\":f1;" << endl;
					_Display(node->Right);
				}
			}
		}

		_Node *_root;			///< 二叉查找树的根结点
	};

	int test()
	{
		BinarySearchTree bst;
		//用随机值生成一棵二叉查找树
		for (int i = 0; i < 100; ++i)
		{
			bst.Insert(rand() % 100);						
		}
		bst.Display();

		//删除所有的奇数值结点
		for (int i = 1; i < 100; i += 2)
		{
			if(bst.Delete(i))
			{
				cout << "### Deleted [" << i << "] ###" << endl;
			}
		}
		bst.Display();

		//查找100以内的数，如果在二叉查找树中，则显示
		cout << endl;
		for (int i = 0; i < 100; i += 1)
		{
			if (bst.Search(i))
			{
				cout << "搜索[" << i << "]元素：" << boolalpha << true << endl;
			}			
		}

		return 0;
	}
}