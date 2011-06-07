#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <iomanip>
#include <limits>
#include <fstream>
#include <string>
#include <numeric>
#include <set>
#include <map>
#include <sstream>
#include <cassert>
#include "graphviz_shower.h"
using namespace std;

namespace chapter18
{
    struct BTreeNode
    {
        vector<int>			Keys;
        vector<BTreeNode *>	Childs;
		BTreeNode			*Parent;		///< 父结点。当该结点是树的根结点时，Parent结点为NULL
        bool				IsLeaf;			///< 是否为叶子结点

		BTreeNode() : Parent(NULL), IsLeaf(true){}

        size_t KeysSize()
        {
            return Keys.size();
        }
    };

    /// B树
    class BTree
    {
    public:
        BTree(int t) : _root( NULL ), _t(t)
        {

        }

        ~BTree()
        {
            _ReleaseNode( _root );
        }

        pair<BTreeNode *, size_t> Search( int key )
        {
            return _SearchInNode( _root, key );
        }

        bool Insert( int new_key )
		{
			if ( _root == NULL )	//空树
			{
				_root = new BTreeNode();
				_root->IsLeaf = true;
				_root->Keys.push_back(new_key);
				return true;
			}

			if ( Search( new_key ).first == NULL )	//是否已经存在该结点
			{
				BTreeNode *node = _root;
				while (!node->IsLeaf)
				{
					int index = 0;
					while ( index < node->Keys.size() && new_key >= node->Keys[index] )
					{
						++index;
					}
					node = node->Childs[index];
				}

				//插入到Keys里去
				node->Keys.insert( find_if( node->Keys.begin(), node->Keys.end(), bind2nd(greater<int>(), new_key) ), new_key );

				//再递归向上处理结点太大的情况
				while (node->KeysSize() > 2 * _t - 1)
				{
					//=====开始分裂======
					int prove_node_key = node->Keys[node->KeysSize() / 2 - 1];			///< 要提升的结点的key

					//后半部分成为一个新节点
					BTreeNode *new_node = new BTreeNode();
					new_node->IsLeaf = node->IsLeaf;					
					new_node->Keys.insert(new_node->Keys.begin(), node->Keys.begin() + node->KeysSize() / 2, node->Keys.end());
					new_node->Childs.insert(new_node->Childs.begin(), node->Childs.begin() + node->Childs.size() / 2, node->Childs.end());
					assert(new_node->Childs.empty() || new_node->Childs.size() == new_node->Keys.size() + 1);
					for_each(new_node->Childs.begin(), new_node->Childs.end(), [&](BTreeNode *c){
						c->Parent = new_node;
					});

					//把后半部分从原来的节点中删除
					node->Keys.erase(node->Keys.begin() + node->KeysSize() / 2 - 1, node->Keys.end());
					node->Childs.erase(node->Childs.begin() + node->Childs.size() / 2, node->Childs.end());		
					assert(node->Childs.empty() || node->Childs.size() == node->Keys.size() + 1);

					BTreeNode *parent_node = node->Parent;
					if (parent_node == NULL)	//分裂到了根结点，树要长高了，需要NEW一个结点出来
					{
						parent_node = new BTreeNode();
						parent_node->IsLeaf = false;
						parent_node->Childs.push_back(node);
						_root = parent_node;
					}
					node->Parent = new_node->Parent = parent_node;

					auto insert_pos = find_if( parent_node->Keys.begin(), parent_node->Keys.end(), bind2nd(greater<int>(), prove_node_key) ) - parent_node->Keys.begin();
					parent_node->Keys.insert( parent_node->Keys.begin() + insert_pos, prove_node_key );
					parent_node->Childs.insert(parent_node->Childs.begin() + insert_pos + 1, new_node);

					node = parent_node;
				}

				return true;
			}
			return false;
		}

        bool Delete( int key_to_del )
		{
			auto found_node = Search(key_to_del);
			if (found_node.first == NULL)		//找不到值为key_to_del的结点
			{
				return false;
			}

			if (!found_node.first->IsLeaf)		//当要删除的结点不是叶子结点时用它的前驱来替换，再删除它的前驱
			{
				//前驱
				BTreeNode *previous_node = found_node.first->Childs[found_node.second];
				while (!previous_node->IsLeaf)
				{
					previous_node = previous_node->Childs[previous_node->Childs.size() - 1];
				}
				
				//替换
				found_node.first->Keys[found_node.second] = previous_node->Keys[previous_node->Keys.size() - 1];
				found_node.first = previous_node;
				found_node.second = previous_node->Keys.size() - 1;
			}

			//到这里，found_node一定是叶子结点
			assert(found_node.first->IsLeaf);
			_DeleteLeafNode(found_node.first, found_node.second);

			return true;
		}

		void Display()
		{
			stringstream ss;

			ss << "digraph graphname" << ( rand() % 1000 ) << "{" << endl
				<< "    node [shape = record,height = .1];" << endl;
			_GetDotLanguageViaNodeAndEdge(ss, _root);
			ss << "}" << endl;

			qi::ShowGraphvizViaDot(ss.str());
		}


    private:
        void _ReleaseNode( BTreeNode *node )
        {
            for_each( node->Childs.begin(), node->Childs.end(), [&]( BTreeNode *c )
            {
                _ReleaseNode( c );
            } );
            delete node;
        }

		/// 删除B树中的一个叶子结点
		void _DeleteLeafNode(BTreeNode *node, size_t index)
		{
			assert(node && node->IsLeaf);

			if (node == _root)
			{
				//要删除的值在根结点上，并且此时根结点也是叶子结点，因为本方法被调用时要保证node参数是叶子结点
				_root->Keys.erase(_root->Keys.begin() + index);
				if (_root->Keys.empty())
				{
					//成为了一棵空B树
					delete _root;
					_root = NULL;
				}
				return;
			}

			//以下是非根结点的情况

			if (node->Keys.size() > _t - 1)
			{	
				//要删除的结点中Key的数目>t-1，因此再-1也不会打破B树的性质
				node->Keys.erase(node->Keys.begin() + index);
			}
			else	//会打破平衡
			{	
				//是否借到了一个顶点
				bool		borrowed = false;		

				//试着从左兄弟借一个结点
				BTreeNode	*left_brother = _GetLeftBrother(node);								
				if (left_brother && left_brother->Keys.size() > _t - 1)	
				{
					int index_in_parent = _GetIndexInParent(left_brother);
					BTreeNode *parent = node->Parent;

					node->Keys.insert(node->Keys.begin(), parent->Keys[index_in_parent]);
					parent->Keys[index_in_parent] = left_brother->Keys[left_brother->KeysSize() - 1];
					left_brother->Keys.erase(left_brother->Keys.end() - 1);

					++index;
					borrowed = true;
				}
				else
				{
					//当左兄弟借不到时，试着从右兄弟借一个结点
					BTreeNode	*right_brother = _GetRightBrother(node);
					if (right_brother && right_brother->Keys.size() > _t - 1)
					{
						int index_in_parent = _GetIndexInParent(node);
						BTreeNode *parent = node->Parent;

						node->Keys.push_back(parent->Keys[index_in_parent]);
						parent->Keys[index_in_parent] = right_brother->Keys[0];
						right_brother->Keys.erase(right_brother->Keys.begin());

						borrowed = true;
					}
				}
								
				if (borrowed)
				{
					//因为借到了结点，所以可以直接删除结点
					_DeleteLeafNode(node, index);
				}
				else
				{
					//左右都借不到时先删除再合并
					node->Keys.erase(node->Keys.begin() + index);
					_UnionNodes(node);
				}
			}
		}

		/// node找一个相邻的结点进行合并
		void _UnionNodes( BTreeNode * node ) 
		{
			if (node)
			{
				if (node == _root)	//node是头结点
				{
					if (_root->Keys.empty())
					{
						//头结点向下移动一级，此时树的高度-1
						_root = _root->Childs[0];
						_root->Parent = NULL;

						delete node;
						return;
					}
				}
				else
				{
					if (node->KeysSize() < _t - 1)
					{					
						BTreeNode *left_brother = _GetLeftBrother(node);
						if (left_brother == NULL)
						{
							left_brother = _GetRightBrother(node);
							swap(node, left_brother);
						}

						//与左兄弟进行合并
						int index_in_parent = _GetIndexInParent(left_brother);
						node->Keys.insert(node->Keys.begin(), node->Parent->Keys[index_in_parent]);
						node->Parent->Keys.erase(node->Parent->Keys.begin() + index_in_parent);
						node->Parent->Childs.erase(node->Parent->Childs.begin() + index_in_parent + 1);
						left_brother->Keys.insert(left_brother->Keys.end(), node->Keys.begin(), node->Keys.end());
						left_brother->Childs.insert(left_brother->Childs.begin(), node->Childs.begin(), node->Childs.end());
						for_each(left_brother->Childs.begin(), left_brother->Childs.end(), [&](BTreeNode *c){
							c->Parent = left_brother;
						});

						delete node;
						_UnionNodes(left_brother->Parent);
					}
				}
			}
		}

		pair<BTreeNode *, size_t> _SearchInNode( BTreeNode *node, int key )
		{
			if ( !node )
            {
                //未找到，树为空的情况
                return make_pair( static_cast<BTreeNode *>(NULL), 0 );
            }
            else
            {
                int index = 0;
                while ( index < node->Keys.size() && key >= node->Keys[index] )
                {
                    if ( key == node->Keys[index] )
                    {
                        return make_pair( node, index );
                    }
                    else
                    {
                        ++index;
                    }
                }

				if (node->IsLeaf)
				{
					//已经找到根了，不能再向下了未找到
					return make_pair( static_cast<BTreeNode *>(NULL), 0 );
				}
				else
				{
					return _SearchInNode( node->Childs[index], key );
				}
            }
        }

		void _GetDotLanguageViaNodeAndEdge(stringstream &ss, BTreeNode *node)
		{
			if (node && !node->Keys.empty())
			{			
				int index = 0;
				ss << "    node" << node->Keys[0] << "[label = \"";
				while (index < node->Keys.size())
				{
					ss << "<f" << 2 * index << ">|";
					ss << "<f" << 2 * index + 1 << ">" << node->Keys[index] << "|";
					++index;
				}
				ss << "<f" << 2 * index << ">\"];" << endl;;

				if (!node->IsLeaf)
				{
					for(int i = 0; i < node->Childs.size(); ++i)
					{
						BTreeNode *c = node->Childs[i];
						ss << "    \"node" << node->Keys[0] << "\":f" << 2 * i << " -> \"node" << c->Keys[0] << "\":f" << (2 * c->Keys.size() + 1) / 2 << ";" << endl;
					}
				}

				for_each(node->Childs.begin(), node->Childs.end(), [&](BTreeNode *c){
					_GetDotLanguageViaNodeAndEdge(ss, c);
				});
			}
		}

		/// 得到一个结点的左兄弟结点，如果不存在左兄弟结点则返回NULL
		BTreeNode * _GetLeftBrother(BTreeNode *node)
		{
			if (node && node->Parent)
			{
				BTreeNode *parent = node->Parent;
				for (int i = 1; i < parent->Childs.size(); ++i)
				{
					if (parent->Childs[i] == node)
					{
						return parent->Childs[i - 1];					
					}
				}
			}
			return NULL;
		}

		/// 得到一个结点的右兄弟结点，如果不存在右兄弟结点则返回NULL
		BTreeNode * _GetRightBrother(BTreeNode *node)
		{
			if (node && node->Parent)
			{
				BTreeNode *parent = node->Parent;
				for (int i = 0; i < static_cast<int>(parent->Childs.size()) - 1; ++i)
				{
					if (parent->Childs[i] == node)
					{
						return parent->Childs[i + 1];
					}
				}
			}
			return NULL;
		}

		/// 得到一个结点在其父结点中属于第几个子结点
		/// @return	返回-1时表示错误
		int _GetIndexInParent( BTreeNode *node )
		{
			assert(node && node->Parent);

			for (int i = 0; i < node->Parent->Childs.size(); ++i)
			{
				if (node->Parent->Childs[i] == node)
				{
					return i;
				}
			}

			return -1;
		}


        BTreeNode	*_root;			///< B树的根结点指针
		int			_t;				///< B树的t值。即所有的结点的Keys的个数应该[_t-1, 2*_t - 1]，除了根结点可以最少为1个Key
    };


	void test()
	{
		BTree btree(3);

		for (int i = 0; i < 20; ++i)
		{
			btree.Insert(rand() % 100 + 1);			
		}
		btree.Display();
		for (int i = 1; i <= 100; i += 2)
		{
			btree.Delete(i);
		}
		btree.Display();
		for (int i = 0; i <= 100; i += 2)
		{
			btree.Delete(i);
		}
		btree.Display();
		for (int i = 0; i < 30; ++i)
		{
			btree.Insert(rand() % 1000 + 1);			
		}
		//btree.Delete(501);
		////btree.Display();
		//btree.Delete(869);
		//btree.Delete(896);
		//btree.Display();
		//btree.Delete(828);
		btree.Display();
	}
}