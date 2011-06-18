//////////////////////////////////////////////////////////////////////////
/// @file		huffman_code.cpp
/// @brief		贪心算法：Huffman编码问题
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
/// 2011/06/17   15:52	1.0	谭川奇	创建

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

namespace ita
{
    namespace
    {
        struct Node
        {
            Node *Left, *Right;
            char	Char;
            double	Value;
        };

        void ShowLeaf( Node * root, string const &code )
        {
            if ( root )
            {
                if ( !root->Left && !root->Right )
                {
                    //叶子结点了
                    cout << root->Char << " ===> " << code << endl;
                }
                else
                {
                    ShowLeaf( root->Left, code + "0" );
                    ShowLeaf( root->Right, code + "1" );
                }
            }
        }
    }

    /// @brief 使用贪心算法解决Huffman编码问题
    ///
    /// 它的原理就是用不等长的编码来表示不同出现频率的字符。出现频率高的字符，就用比较短的编码来表示，出现频率低的，就是较长的编码来表示。
    void HuffmanCode()
    {
        string orgin_str = "abcadbabacadbacdabadeabadabaewbadsafdsawebadsfasdfababadasdfabababcadb";
        map<char, double> frequency;
        for_each( orgin_str.begin(), orgin_str.end(), [&]( char const & c )
        {
            ++frequency[c];
        } );

        vector<Node *> nodes;
        for_each( frequency.begin(), frequency.end(), [&]( pair<char const, double> &item )
        {
            item.second /= orgin_str.size();

            Node *node = new Node();
            node->Value = item.second;
            node->Char = item.first;
            nodes.push_back( node );
        } );

        for_each( frequency.begin(), frequency.end(), [&]( pair<char const, double> &item )
        {
            cout << item.first << ":  " << item.second << endl;
        } );

        while ( nodes.size() > 1 )
        {
            //有超过一个结点，则循环
            sort( nodes.begin(), nodes.end(), []( Node * n1, Node * n2 )
            {
                return n1->Value < n2->Value;
            } );

            Node *new_node = new Node();
            new_node->Left = nodes[0];
            new_node->Right = nodes[1];
            new_node->Value = nodes[0]->Value + nodes[1]->Value;

            nodes.erase( nodes.begin(), nodes.begin() + 2 );
            nodes.push_back( new_node );
        }

        ShowLeaf( nodes[0], "" );
        getchar();
    }




}