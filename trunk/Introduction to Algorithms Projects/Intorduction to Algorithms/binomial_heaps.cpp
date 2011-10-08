//////////////////////////////////////////////////////////////////////////
/// @file		binomial_heaps.cpp
/// @brief		二项堆
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
/// 2011/06/17   15:47	1.0	谭川奇	创建

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include "graphviz_shower.h"
using namespace std;

namespace ita
{
    /// @brief 二项堆
    ///
    /// 二项堆是由<b>一组</b>满足下面的<b>二项树</b>组成：\n
    /// - H中的每个二项树遵循<span style="color:#FF0000 ">最小堆性质：结点的关键字大于或等于其父结点的关键字</span>。
    /// 我们说这种树是最小堆有序的。
    /// - 对于任意的整数k的话，在H不存在另外一个度数也是k的二项树；即<span style="color:#FF0000 ">至多</span>（有0或1棵）
    /// 有一棵二项树的根具有度数K。（与二进制太类似了，估计是受二进制启发而来的）
    ///
    /// 第一个性质保证了二项树的根结点包含了最小的关键字。第二个性质则说明结点数为n的二项堆最多只有logn + 1棵二项树。\n
    /// 二项树是一种递归的定义:\n
    /// - 二项树B[0]仅仅包含一个节点
    /// - B[k]是由两棵B[k-1]二项树组成，其中一颗树是另外一颗树的子树。
    template<typename KeyType>
    class BinomialHeap
    {
    public:
        /// 二项堆中的一个结点
        struct BinomialHeapNode
        {
            BinomialHeapNode	*Parent;
            BinomialHeapNode	*Child;
            BinomialHeapNode	*Sibling;

            unsigned int 		Degree;
            KeyType				Key;
        };

        /// 建立一个空的二项堆
        BinomialHeap()
        {
            _head_list = nullptr;
        }

        /// 析构
        ~BinomialHeap()
        {
            BinomialHeapNode *tree = _head_list;

            //遍历根链进行清理操作
            while( tree )
            {
                auto next = tree->Sibling;
                _DeleteTree( tree );
                tree = next;
            }
        }

        /// 在二项堆中插入一个新结点
        void Insert( KeyType new_key )
        {
            BinomialHeap new_heap;
            new_heap._head_list = new BinomialHeapNode();
            new_heap._head_list->Parent = new_heap._head_list->Child = new_heap._head_list->Sibling = nullptr;
            new_heap._head_list->Degree = 0;
            new_heap._head_list->Key = new_key;

            this->Union( new_heap );
        }

        /// 获取堆中的最小元素值
        KeyType Mininum() const
        {
            vector<KeyType> values_in_head_list;
            BinomialHeapNode *node = _head_list;
            while( node != nullptr )
            {
                values_in_head_list.push_back( node->Key );
                node = node->Sibling;
            }
            return *min_element( values_in_head_list.begin(), values_in_head_list.end() );
        }

        /// 弹出二项堆中的最小元素，并获取该最小元素的值
        KeyType ExtractMinium()
        {
            vector<BinomialHeapNode *> head_nodes;
            BinomialHeapNode *l = _head_list;
            while( l )
            {
                head_nodes.push_back( l );
                l = l->Sibling;
            }
            auto min_ele = min_element( head_nodes.begin(), head_nodes.end(), []( BinomialHeapNode * left, BinomialHeapNode * right )
            {
                return left->Key < right->Key;
            } );
            int min_index = min_ele - head_nodes.begin();
            KeyType min_value = ( *min_ele )->Key;
            BinomialHeapNode	*min_node = head_nodes[min_index];

            if ( min_index == 0 )
            {
                if ( head_nodes.size() > 1 )
                {
                    _head_list = head_nodes[1];
                }
                else
                {
                    //根链上只有一个元素
                    _head_list = nullptr;
                }
            }
            else if ( min_index == head_nodes.size() - 1 )
            {
                head_nodes[min_index - 1]->Sibling = nullptr;
            }
            else
            {
                head_nodes[min_index - 1]->Sibling = head_nodes[min_index + 1];
            }

            BinomialHeap new_head;
            new_head._head_list = min_node->Child;
            BinomialHeapNode *x = new_head._head_list;
            while ( x )
            {
                x->Parent = nullptr;
                x = x->Sibling;
            }
            this->Union( new_head );

            delete min_node;
            return min_value;
        }

        /// @brief 对一个结点的值进行缩小操作
        ///
        /// @note		x结点的新值k必须比x结点的原值小
        void Decrease( BinomialHeapNode *x, KeyType k )
        {
            if ( k > x->Key )
            {
                throw exception( "只能减小不能增大" );
            }

            x->Key = k;
            BinomialHeapNode *y = x;
            BinomialHeapNode *z = y->Parent;
            while ( z && y->Key < z->Key )
            {
                swap( y->Key, z->Key );
                y = z;
                z = y->Parent;
            }
        }

        /// 删除一个结点
        void Delete( BinomialHeapNode *node )
        {
            if ( node )
            {
                Decrease( node, numeric_limits<KeyType>::min() );
                ExtractMinium();
            }
        }

        /// @brief 联合另外一个二项堆
        ///
        /// 将另外一个二项堆联合到本二项堆,当联合操作完成之后,other的二项堆中的数据将无效
        void Union( BinomialHeap &other )
        {
            vector<BinomialHeapNode *> nodes;
            BinomialHeapNode *l = _head_list;
            BinomialHeapNode *r = other._head_list;
            while ( l )
            {
                nodes.push_back( l );
                l = l->Sibling;
            }
            while ( r )
            {
                nodes.push_back( r );
                r = r->Sibling;
            }
            if ( nodes.empty() )
            {
                return;
            }

            // sort and merge
            sort( nodes.begin(), nodes.end(), []( BinomialHeapNode * left, BinomialHeapNode * right )
            {
                return left->Degree < right->Degree;
            } );
            for ( size_t i = 0; i < nodes.size() - 1; ++i )
            {
                nodes[i]->Sibling = nodes[i + 1];
            }
            nodes[nodes.size() - 1]->Sibling = nullptr;

            // reset head list
            this->_head_list = nodes[0];
            other._head_list = nullptr;		//destory binomial heaps has been unioned
            if ( this->_head_list == nullptr )
            {
                return;
            }

            // begin to merge nodes in head list with same degree
            BinomialHeapNode *prev_x = nullptr;
            BinomialHeapNode *x = _head_list;
            BinomialHeapNode *next_x = x->Sibling;

            while( next_x )
            {
                if ( x->Degree != next_x->Degree || ( next_x->Sibling != nullptr && next_x->Sibling->Degree == x->Degree ) )
                {
                    prev_x = x;
                    x = next_x;
                }
                else if ( x->Key < next_x->Key )
                {
                    x->Sibling = next_x->Sibling;
                    _Link( next_x, x );
                }
                else
                {
                    if ( prev_x == nullptr )
                    {
                        _head_list = next_x;
                    }
                    else
                    {
                        prev_x->Sibling = next_x;
                    }

                    _Link( x, next_x );
                    x = next_x;
                }

                next_x = x->Sibling;
            }
        }

        /// @brief 查找一个值为key的结点
        ///
        /// @note		所有的堆对查找操作的支持都很差，时间复杂度为O(n)
        BinomialHeapNode * Search( KeyType key ) const
        {
            BinomialHeapNode *tree = _head_list;

            //遍历根链
            while( tree )
            {
                BinomialHeapNode *node = _SearchInTree( tree, key );
                if ( node != nullptr )
                {
                    return node;
                }
                tree = tree->Sibling;
            }
            return nullptr;
        }

        /// 二项堆的当前状态是否为空
        bool IsEmpty() const
        {
            return _head_list == nullptr;
        }

        /// 得到二项堆的根链表
        BinomialHeapNode * GetHeadList()
        {
            return _head_list;
        }

        /// 使用Grpahviz显示当前二项堆
        void Display() const
        {
            stringstream ss;

            ss << "digraph graphname" << "{" << endl
               << "    RootList [shape = box];" << endl;

            BinomialHeapNode *node = _head_list;
            if ( node )
            {
                ss << "    RootList -> " << node->Key << ";" << endl;
            }
            while( node )
            {
                _DisplayTree( ss, node );

                if ( node->Sibling )
                {
                    ss << "    " << node->Key << " -> " << node->Sibling->Key << ";" << endl;
                }
                node = node->Sibling;
            }

            ss << "}" << endl;
            qi::ShowGraphvizViaDot( ss.str() );

        }

    private:
        /// 清理一棵“二项树”
        void _DeleteTree( BinomialHeapNode *tree )
        {
            if ( tree )
            {
                BinomialHeapNode *node = tree->Child;
                while( node )
                {
                    auto next = node->Sibling;
                    _DeleteTree( node );
                    node = next;
                }

                delete tree;
            }
        }

        /// 将D(k-1)度的y结点连接到D(k-1)度的z接点上去，使得z成为一个D(k)度的结点
        void _Link( BinomialHeapNode *y, BinomialHeapNode *z )
        {
            y->Parent = z;
            y->Sibling = z->Child;
            z->Child = y;
            ++z->Degree;
        }

        /// 在一棵二项树中搜索某个结点
        BinomialHeapNode * _SearchInTree( BinomialHeapNode *tree, KeyType key ) const
        {
            if ( tree->Key == key )
            {
                return tree;
            }

            BinomialHeapNode *node = tree->Child;
            while( node )
            {
                BinomialHeapNode *n = _SearchInTree( node, key );
                if ( n )
                {
                    return n;
                }
                node = node->Sibling;
            }

            return nullptr;
        }

        /// 画一棵二项树
        void _DisplayTree( stringstream &ss, BinomialHeapNode *tree ) const
        {
            if ( tree )
            {
                BinomialHeapNode *child = tree->Child;
                if ( child )
                {
                    vector<BinomialHeapNode *> childs;
                    while ( child )
                    {
                        childs.push_back( child );
                        child = child->Sibling;
                    }

                    for_each( childs.begin(), childs.end(), [&]( BinomialHeapNode * c )
                    {
                        ss << "    " << c->Key << " -> " << tree->Key << ";" << endl;
                        _DisplayTree( ss, c );
                    } );
                }
            }
        }

    private:
        BinomialHeapNode *_head_list;			///< 根链表
    };


    /// 二项堆
    void testBinomialHeaps()
    {
        cout << "二项堆" << endl;
        BinomialHeap<int> bh;
        for ( int i = 0; i < 10; ++i )
        {
            bh.Insert( rand() % 100 );
        }
        bh.Display();

        while ( !bh.IsEmpty() )
        {
            cout << bh.ExtractMinium() << "\t";
        }
        bh.Display();

        for ( int i = 0; i < 64; ++i )
        {
            int r = rand() % 100;
            if ( bh.Search( r ) == nullptr )
            {
                bh.Insert( r );
            }
        }
        bh.Display();

        for ( int i = 0; i < 100; ++i )
        {
            bh.Delete( bh.Search( i ) );
        }
        bh.Display();
    }
}