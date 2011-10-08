//////////////////////////////////////////////////////////////////////////
/// @file		red_black_tree.cpp
/// @brief		红黑树
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
/// 2011/06/17   13:23	1.0	谭川奇	创建


#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <iomanip>
#include <limits>
#include <sstream>
#include "graphviz_shower.h"
using namespace std;

namespace ita
{
	/// @brief 红黑树
	///
	/// 满足下面几个条件(红黑性质)的二叉搜索树，称为红黑树：
	/// -# 每个结点或是红色，或是是黑色。
	/// -# 根结点是黑的。
	/// -# 所有的叶结点(NULL)是黑色的。（NULL被视为一个哨兵结点，所有应该指向NULL的指针，都看成指向了NULL结点。）
	/// -# 如果一个结点是红色的，则它的两个儿子节点都是黑色的。
	/// -# 对每个结点，从该结点到其子孙结点的所有路径上包含相同数目的黑结点。
	///
	/// 红黑树的性质：
	/// - 黑高度的定义： 从某个结点出发(不包括该结点)到达一个叶结点的任意一条路径上，黑色结点的个数成为该结点x的黑高度。
	/// 红黑树的黑高度定义为其根结点的黑高度。
	/// - 红黑树是真正的在实际中得到大量应用的复杂数据结构：C++STL中的关联容器map,set都是红黑树的应用（所以标准库容器的
	/// 效率太好了，能用标准库容器尽量使用标准库容器）；\n Linux内核中的用户态地址空间管理也使用了红黑树。
	/// - 红黑树是许多“<b>平衡的</b>”查找树中的一种（首先：<span style="color:#FF0000 ">红黑树是一种近似平衡的二叉树
	/// </span>），它能保证在最坏的情况下，基本的动态集合操作的时间为O(lgn)。
	/// - 通过对任何一条从根到叶子的路径上各个结点着色方式的限制，红黑树确保没有一条路径会比其它路径长出两倍，因而是接近平衡的。
	/// - 一要全是黑结点的满二叉树也满足红黑树的定义。满二叉树的效率本身就非常高啊，它是效率最好的二叉树了，所以说它是
	/// 红黑树的一个特例；普通的红黑树要求并没有满二叉树这么严格。
	/// - 红黑树之所以这么高效，是因为它是<span style="color:#FF0000 ">近似平衡</span>的，又不要求完全的平衡，减少了维
	/// 护的代价。在计算机科学中有大量的这样的例子，使用近似的东西来提高效率。如二项堆、斐波那契堆等等数不胜数…
	/// @param	TKey	结点中键的类型
	/// @param	TValue	结点中值的类型
	template<typename TKey, typename TValue>
	class RBTree
	{
	public:
		/// 红黑树中结点颜色的枚举
		enum RBTreeNodeColor
		{
			BLACK,			///< 黑色
			RED				///< 红色
		};

		/// 红黑树中的结点
		struct RBTreeNode
		{
			TKey				Key;			///< 结点中的KEY
			TValue				Value;			///< 结点中的值
			RBTreeNodeColor		Color;			///< 结点的颜色，红色还是黑色
			RBTreeNode			*Parent;		///< 父结点指针
			RBTreeNode			*Left;			///< 左孩子指针
			RBTreeNode			*Right;			///< 右孩子指针

			/// @brief 检查是否有效（哨兵结点nil 算作无效结点）
			/// @return			该结点是否为有效结点，即不为nil结点
			/// @retval	true	非nil结点
			/// @retval false	nil结点
			inline bool IsValid() const
			{
				return ( this != s_nil );
			}
		};

		RBTree()
		{
			if ( !s_nil )
			{
				//叶子结点是一个特殊的黑结点
				s_nil = new RBTreeNode();
				s_nil->Color = BLACK;
			}

			_root = s_nil;
		}

		~RBTree()
		{
			_RecursiveReleaseNode( _root );
		}

		/// @brief 插入一个结点
		///
		/// 红黑树的插入操作：将结点Z插入树T中，就好像T是一棵普通的二叉查找树一样，然后将Z着为红色。为保证红黑性质能继续
		/// 保持，我们调用一个辅助程序来对结点重新着色并旋转。这么做是有它的智慧的：首先，插入结点Z的位置的确应该和普通
		/// 二叉查找树一样，因为红黑树本身就首先是一棵二叉查找树；然后将Z着为红色，是为了保证性质5的正确性，因为性质5如
		/// 果被破坏了是最难以恢复的；到这里，有可能被破坏的性质就只剩下性质2和性质4了，这都可以通过后来的辅助程序进行修
		/// 复的。\n
		/// 插入操作可能破坏的性质：
		/// - 性质2：当被一棵空树进行插入操作时发生；
		/// - 性质4：当新结点被插入到红色结点之后时发生；
		bool Insert( TKey key, TValue value )
		{
			if ( Search( key )->IsValid() )
			{
				//key重复，添加失败
				return false;
			}
			else
			{
				//新添加的结点为红结点，且Left=Right=s_nil
				RBTreeNode *new_node = new RBTreeNode();
				new_node->Key = key;
				new_node->Value = value;
				new_node->Color = RED;
				new_node->Left = new_node->Right = s_nil;

				_InsertAsNormalBSTree( new_node );
				_InsertFixup( new_node );
				return true;
			}
		}

		/// @brief 删除一个结点
		/// 红黑树的删除操作：和插入操作一样，先用BST的删除结点操作，然后调用相应的辅助函数做相应的调整。\n
		/// 首先只有被删除的结点为黑结点时才需要进行修补，理由如下：
		/// - 树中各结点的黑高度都没有变化
		/// - 不存在两个相邻的红色结点
		/// - 因为如果被删除的点是红色，就不可能是根，所以根仍然是黑色的
		///
		/// 当被删除了黑结点之后，红黑树的性质5被破坏，上面说过了性质5被破坏后的修复难度是最大的。所以这里的修复过程使用
		/// 了一个很新的思想，即视为被删除的结点的子结点有额外的一种黑色，当这一重额外的黑色存在之后，性质5就得到了继续
		/// 。然后再通过转移的方法逐步把这一重额外的黑色逐渐向上转移直到根或者红色的结点，最后消除这一重额外的黑色。\n
		/// 删除操作中可能被破坏的性质：
		/// - 性质2：当y是根时，且y的一个孩子是红色，若此时这个孩子成为根结点；
		/// - 性质4：当x和p[y]都是红色时；
		/// - 性质5：包含y的路径中，黑高度都减少了；
		bool Delete( TKey key )
		{
			RBTreeNode *z = Search( key );
			if ( z->IsValid() )
			{
				//实际要删除的结点，因为后面会有一个交换，所以实际删除y之后就达到了z的效果
				RBTreeNode *y = nullptr;
				if ( !z->Left->IsValid() || !z->Right->IsValid() )
				{
					//至少有一个孩子为nil
					y = z;
				}
				else
				{
					//左右孩子均不为nil，则找后继
					y = _Successor( z );
				}

				RBTreeNode *x = ( y->Left->IsValid() ? y->Left : y->Right );
				x->Parent = y->Parent;

				if ( !y->Parent->IsValid() )
				{
					_root = x;
				}
				else
				{
					if ( y == y->Parent->Left )
					{
						y->Parent->Left = x;
					}
					else
					{
						y->Parent->Right = x;
					}
				}

				if ( y != z )
				{
					z->Key = y->Key;
					z->Value = y->Value;
				}

				if ( y->Color == BLACK )
				{
					_DeleteFixup( x );
				}

				delete y;	//最后实际删除了y结点
				return true;
			}
			else
			{
				//要删除的结点不存在
				return false;
			}
		}

		/// 在红黑树上搜索一个结点
		RBTreeNode * Search( TValue const &value )
		{
			RBTreeNode *node = _root;
			while ( node != s_nil && node->Value != value )
			{
				node = ( value < node->Value ? node->Left : node->Right );
			}
			return node;
		}

		/// 判断红黑树是否为空
		bool Empty()
		{
			return !( _root->IsValid() );
		}

		/// @brief 显示当前二叉查找树的状态
		void Display() const
		{
			stringstream ss;

			ss << "digraph graphname" << ( rand() % 1000 ) << "{" << endl
				<< "    node [shape = record,height = .1];" << endl;
			_Display( ss, _root );
			ss << "}" << endl;

			qi::ShowGraphvizViaDot( ss.str() );
		}

	private:
		void _RecursiveReleaseNode( RBTreeNode *node )
		{
			if ( node->IsValid() )
			{
				_RecursiveReleaseNode( node->Left );
				_RecursiveReleaseNode( node->Right );
				delete node;
			}
		}

		void _Display( stringstream &ss, RBTreeNode *node ) const
		{
			if ( node->IsValid() )
			{
				ss << "    node" << node->Value << "[label = \"<f0>|<f1>" << node->Value << "|<f2>\", color = " << ( node->Color == RED ? "red" : "black" ) << "];" << endl;

				if ( node->Left->IsValid() )
				{
					ss << "    \"node" << node->Value << "\":f0 -> \"node" << node->Left->Value << "\":f1;" << endl;
					_Display( ss, node->Left );
				}

				if ( node->Right->IsValid() )
				{
					ss << "    \"node" << node->Value << "\":f2 -> \"node" << node->Right->Value << "\":f1;" << endl;
					_Display( ss, node->Right );
				}
			}
		}

		/// @brief 将一个结点简单地加入红黑树
		///
		/// 视该红黑树为普通的二叉查找树简单的进行插入操作，需要在此之后调整以满足红黑树的性质
		/// @note		一定要保证node->Key一定是一个新的值，否则会无限循环，在这里不检查
		void _InsertAsNormalBSTree( RBTreeNode *node )
		{
			if ( !_root->IsValid() )
			{
				//插入的是第1个节点
				_root = node;
				_root->Left = _root->Right = _root->Parent = s_nil;
				_root->Color = BLACK;
				return;
			}

			//非第1个节点
			RBTreeNode *current_node = _root;

			while ( true )
			{
				RBTreeNode *&next_node_pointer = ( node->Key > current_node->Key ? current_node->Right : current_node->Left );
				if ( next_node_pointer->IsValid() )
				{
					current_node = next_node_pointer;
				}
				else
				{
					//进行真正的插入操作
					node->Parent = current_node;
					next_node_pointer = node;
					break;
				}
			}
		}

		/// @brief 对插入操作的修复
		///
		/// 由于对红黑树的插入操作破坏了红黑树的性质，所以需要对它进行修正
		/// @note		node的结点是需要处理的结点，由于它破坏了红黑性质，它一定是红结点
		void _InsertFixup( RBTreeNode * node )
		{
			while ( node->Parent->Color == RED )
			{
				//标识node的父结点是否为node的祖父结点的左孩子
				bool parent_is_left_child_flag = ( node->Parent == node->Parent->Parent->Left );
				//叔叔结点
				RBTreeNode *uncle = parent_is_left_child_flag ? node->Parent->Parent->Right : node->Parent->Parent->Left;
				if ( uncle->Color == RED )
				{
					//case1
					node->Parent->Color = BLACK;
					uncle->Color = BLACK;
					node->Parent->Parent->Color = RED;
					node = node->Parent->Parent;
				}
				else
				{
					if ( node == ( parent_is_left_child_flag ? node->Parent->Right : node->Parent->Left ) )
					{
						//case2
						node = node->Parent;
						parent_is_left_child_flag ? _LeftRotate( node ) : _RightRotate( node );
					}

					//case3
					node->Parent->Color = BLACK;
					node->Parent->Parent->Color = RED;
					parent_is_left_child_flag ? _RightRotate( node->Parent->Parent ) : _LeftRotate( node->Parent->Parent );
				}
			}

			//处理性质2被破坏只需要简简单单一句话
			_root->Color = BLACK;
		}

		/// 左旋
		///
		/// 旋转操作是一种能保持二叉查找树性质的查找树局部操作
		void _LeftRotate( RBTreeNode * node )
		{
			if ( !( node->IsValid() && node->Right->IsValid() ) )
			{
				//左旋操作要求对非哨兵进行操作，并且要求右孩子也不是哨兵
				throw std::exception( "左旋操作要求对非哨兵进行操作，并且要求右孩子也不是哨兵" );
			}
			else
			{
				//node的右孩子
				RBTreeNode *right_son = node->Right;

				node->Right = right_son->Left;
				if ( right_son->Left->IsValid() )
				{
					right_son->Left->Parent = node;
				}
				right_son->Parent = node->Parent;
				if ( !( node->Parent->IsValid() ) )
				{
					_root = right_son;
				}
				else
				{
					if ( node == node->Parent->Left )
					{
						node->Parent->Left = right_son;
					}
					else
					{
						node->Parent->Right = right_son;
					}
				}
				right_son->Left = node;
				node->Parent = right_son;
			}
		}

		/// 右旋
		///
		/// 旋转操作是一种能保持二叉查找树性质的查找树局部操作
		void _RightRotate( RBTreeNode * node )
		{
			if ( !( node->IsValid() && node->Left->IsValid() ) )
			{
				//右旋操作要求对非哨兵进行操作，并且要求左孩子也不是哨兵
				throw std::exception( "右旋操作要求对非哨兵进行操作，并且要求左孩子也不是哨兵" );
			}
			else
			{
				//node的左孩子
				RBTreeNode *left_son = node->Left;

				node->Left = left_son->Right;
				if ( left_son->Right->IsValid() )
				{
					left_son->Right->Parent = node;
				}
				left_son->Parent = node->Parent;
				if ( !( node->Parent->IsValid() ) )
				{
					_root = left_son;
				}
				else
				{
					if ( node == node->Parent->Left )
					{
						node->Parent->Left = left_son;
					}
					else
					{
						node->Parent->Right = left_son;
					}
				}
				left_son->Right = node;
				node->Parent = left_son;
			}
		}

		/// 对删除操作的修复
		void _DeleteFixup( RBTreeNode *x )
		{
			while( x != _root && x->Color == BLACK )
			{
				bool node_is_parent_left_child = ( x == x->Parent->Left );
				RBTreeNode *w = node_is_parent_left_child ? x->Parent->Right : x->Parent->Left;

				if( w->Color == RED )
				{
					//case1
					w->Color = BLACK;
					x->Parent->Color = RED;
					_LeftRotate( x->Parent );
					w = x->Parent->Right;
				}

				//case2
				if( w->Left->Color == BLACK && w->Right->Color == BLACK )
				{
					//两个孩子都为黑结点
					w->Color = RED;
					x = x->Parent;
				}
				else
				{
					//case3
					if( ( node_is_parent_left_child ? w->Right->Color : w->Left->Color ) == BLACK )
					{
						( node_is_parent_left_child ? w->Left->Color : w->Right->Color ) = BLACK;
						w->Color = RED;
						node_is_parent_left_child ? _RightRotate( w ) : _LeftRotate( w );
						w = ( node_is_parent_left_child ? x->Parent->Right : x->Parent->Left );
					}

					//case4
					w->Color = x->Parent->Color;
					x->Parent->Color = BLACK;
					( node_is_parent_left_child ? w->Right->Color : w->Left->Color ) = BLACK;
					node_is_parent_left_child ? _LeftRotate( x->Parent ) : _RightRotate( x->Parent );
					x = _root;
				}
			}

			//最后只需要简单置x为黑结点就可以，_root的改变已经由左右旋自动处理了
			x->Color = BLACK;    //改为黑色。
		}

		/// 得到节点的后继
		RBTreeNode * _Successor( RBTreeNode *node )
		{
			if ( node->Right->IsValid() )
			{
				//存在右结点时：右一下，左到头
				node = node->Right;
				while ( node->Left->IsValid() )
				{
					node = node->Left;
				}
				return node;
			}
			else
			{
				//不存在右结点时：一直向上，直到找到一次非右孩子或到根了为止
				RBTreeNode *y = node->Parent;
				while ( !y->IsValid() && node == y->Right )
				{
					node = y;
					y = y->Parent;
				}
				return y;
			}
		}


		RBTreeNode *_root;				///< 根结点
		static RBTreeNode *s_nil;		///< 红黑树的叶子结点（哨兵）
	};


	template<typename TKey, typename TValue>
	typename RBTree<TKey, TValue>::RBTreeNode * RBTree<TKey, TValue>::s_nil = nullptr;

	/// 红黑树
	int testRedBlackTree()
	{
		int init[] = {11, 2, 14, 1, 7, 15, 5, 8};
		RBTree<int, int> bst;
		//for ( int i = 0; i < sizeof( init ) / sizeof( init[0] ); ++i )
		//{
		//    bst.Insert( init[i], init[i] );
		//}
		//bst.Insert( 4, 4 );
		//bst.Display();

		//用随机值生成一棵二叉查找树
		for ( int i = 0; i < 50; ++i )
		{
			int v = rand() % 100;
			bst.Insert( v, v );
		}
		bst.Display();

		//bst.Delete(5);
		//删除所有的小奇数
		for ( int i = 0; i < 100; ++i )
		{
			if ( i % 2 == 1 && i < 50 )
			{
				if ( bst.Delete( i ) )
				{
					cout << "Deleted [" << i << "]" << endl;
				}
			}
		}
		bst.Display();
		//删除所有的大偶数
		for ( int i = 0; i < 100; ++i )
		{
			if ( i % 2 == 0 && i > 50 )
			{
				if ( bst.Delete( i ) )
				{
					cout << "Deleted [" << i << "]" << endl;
				}
			}
		}
		bst.Display();
		//再随机添加
		for ( int i = 0; i < 50; ++i )
		{
			int v = rand() % 100;
			bst.Insert( v, v );
		}
		bst.Display();
		//删除所有
		for ( int i = 0; i < 100; ++i )
		{
			if ( bst.Delete( i ) )
			{
				cout << "Deleted [" << i << "]" << endl;
			}

		}
		//bst.Display();

		for ( int i = 0; i < 50; ++i )
		{
			int v = rand() % 100;
			bst.Insert( v, v );
		}
		bst.Display();

		return 0;
	}
}