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
using namespace std;

namespace chapter17
{
    struct BTreeNode
    {
        vector<int>			Keys;
        vector<BTreeNode *>	Childs;
        bool				IsLeaf;

        size_t KeysSize()
        {
            return Keys.size();
        }
    };

    /// B树
    class BTree
    {
    public:
        BTree() : _root( NULL )
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

        bool Insert();
        bool Delete();


    private:
        void _ReleaseNode( BTreeNode *node )
        {
            for_each( node->Childs.begin(), node->Childs.end(), [&]( BTreeNode *c )
            {
                _ReleaseNode( c );
            } );
            delete node;
        }

        pair<BTreeNode *, size_t> _SearchInNode( BTreeNode *node, int key )
        {
            if ( !node )
            {
                //未找到
                return make_pair( NULL, 0 );
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
                return _SearchInNode( node->Childs[index], key );
            }
        }


        BTreeNode *_root;			///< B树的根结点指针
    };

}