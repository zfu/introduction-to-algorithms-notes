//////////////////////////////////////////////////////////////////////////  
///    COPYRIGHT NOTICE  
///    Copyright (c) 2009
///    All rights reserved.  
///  
/// @file		binary_search_tree.cpp
/// @brief		binary_search_tree.cpp的简短描述 
///  			
///  			binary_search_tree.cpp的详细描述
/// 
/// @author		谭川奇	chuanqi.tan@gmail.com 
/// @date		2011/05/30
/// @version 	1.0
///  
///  
///    修订说明：最初版本  
//////////////////////////////////////////////////////////////////////////  

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
			int Value;
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
		/// @param	new_value	要插入的值
		/// @return				是否插入成功，失败意味着树中已经存在该值
		bool Insert(int const new_value)
		{
			if (Search(new_value))
			{//已经存在
				return false;
			}

			if (!_root)
			{//插入的是第1个节点
				_root = new _Node();
				_root->Value = new_value;
				return true;
			}

			//非第1个节点
			_Node *current_node = _root;

			while (current_node)
			{
				_Node *&next_node_pointer = (new_value > current_node->Value ? current_node->Right : current_node->Left);
				if (next_node_pointer)
				{
					current_node = next_node_pointer;
				}
				else
				{
					next_node_pointer = new _Node();
					next_node_pointer->Value = new_value;
					next_node_pointer->Parent = current_node;
					break;
				}
			}

			return true;
		}

		/// 删除结点
		/// 
		/// 在二叉查找树中删除一个值
		/// @param	delete_value	要删除的值
		/// @return					是否删除成功，删除失败意味着树中没有这个值的结点
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
		/// @param	search_value	要查找的值
		/// @return					是否在二叉树中找到值为search_value的结点
		/// @retval		true		查找到了该元素
		/// @retval		false		找不到该元素
		bool Search(int const search_value) const
		{
			return _Search(_root, search_value) != NULL;
		}

		/// 显示当前二叉查找树的状态
		/// 
		/// 使用dot描述当前二叉查找树
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
				_Node *previous_node = _GetPreviousNode(delete_node);

				delete_node->Value = previous_node->Value;

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
					(delete_node->Parent->Left == delete_node ? delete_node->Parent->Left : delete_node->Parent->Right) = sub_node;

					if (sub_node)
					{//在delete_node有子结点时需要设置子结点的Parent指针
						sub_node->Parent = delete_node->Parent;
					}
				}

				delete delete_node;
			}
		}

		/// 得到一个同时存在左右子树的节点的前驱
		/// 
		/// @notes		node的前驱一定存在，因为node同时存在左子树和右子树
		_Node * _GetPreviousNode( _Node * node )
		{
			if (!node->Left || !node->Right)
			{//先决条件必须满足，否则求无限制的结点的前驱算法不是这样的
				throw std::exception("node必须同时存在左子树和右子树");
			}

			//还是注意先决条件：node是一个同时存在左右子树的结点，否则算法不是这样的
			//求结点的前驱：先左一下，再右到头
			node = node->Left;
			while (node->Right)
			{
				node = node->Right;
			}

			return node;
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

		/// 非递归查找一个结点
		_Node * _Search(_Node *node, int const search_value) const
		{
			while (node && node->Value != search_value)
			{
				if (search_value < node->Value)
				{
					node = node->Left;
				}
				else
				{
					node = node->Right;
				}
			}

			//到这里如果node为空就是未找到
			return node;
		}

		void _Display(_Node *node) const
		{
			if (node)
			{
				cout << "    node" << node->Value << "[label = \"<f0>|<f1>" << node->Value << "|<f2>\"];" << endl;

				if (node->Left)
				{
					cout << "    \"node" << node->Value << "\":f0 -> \"node" << node->Left->Value << "\":f1;" << endl;
					_Display(node->Left);
				}

				if (node->Right)
				{
					cout << "    \"node" << node->Value << "\":f2 -> \"node" << node->Right->Value << "\":f1;" << endl;
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
		for (int i = 0; i < 10; ++i)
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
				cout << "搜索[" << i << "]元素：\t成功" << endl;
			}			
		}

		return 0;
	}
}