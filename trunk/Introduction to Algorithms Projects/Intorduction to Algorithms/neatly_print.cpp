//////////////////////////////////////////////////////////////////////////
/// @file		neatly_print.cpp
/// @brief		整齐打印问题
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
/// 2011/06/17   14:40	1.0	谭川奇	创建

#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <iomanip>
#include <limits>
#include <fstream>
#include <string>
#include <set>
using namespace std;

namespace ita
{
    namespace
    {
        /// @brief 整齐打印的贪心算法
        ///
        /// 可以证明：以于整齐打印问题，贪心算法能够得到最优解！
        int NeatlyPrintViaGreedy( vector<string> const &words, int const max_length )
        {
            int cost = 0;
            set<int> split_pos;

            int current_line_length = 0;
            for ( int i = 0; i < words.size(); ++i )
            {
                int add_char_num = ( current_line_length == 0 ? words[i].size() : words[i].size() + 1 );
                if ( add_char_num + current_line_length <= max_length )
                {
                    //当前行放得下
                    current_line_length += add_char_num;
                }
                else
                {
                    //当前行已经放不下了，换行
                    split_pos.insert( i );
                    cost += pow( ( max_length - current_line_length ), 1.0 );
                    current_line_length = words[i].size();
                }
            }

            current_line_length = 0;
            for ( size_t i = 0; i < words.size(); ++i )
            {
                if ( split_pos.find( i ) != split_pos.end() )
                {
                    //换行
                    string space( max_length - current_line_length, ' ' );
                    cout << space << '|' << endl;
                    current_line_length = 0;
                }

                if ( current_line_length != 0 )
                {
                    cout << ' ';
                }
                cout << words[i];

                current_line_length += ( current_line_length == 0 ? words[i].size() : words[i].size() + 1 );
            }

            return cost;
        }

        /// @brief 错误的整齐打印的动态规划算法
        ///
        /// 这是网上流传的关于本题的解法，经验证是错误的、毫无道理的！\n
        /// 动态规划得到的应该是最优解，而不是次优解。在这里动态规划的结果还没有贪心算法好，这个动态规划是完全错误的！\n
        /// 而且这个所谓的动态规划所使用的解递推公式实在是不可理喻！毫无道理！\n
        int NeatlyPrintViaDyanmicProgrammingWrong( vector<string> const &words, int const max_length )
        {
            //cost[i]表示[0..i]单词的最小代价
            vector<int> cost( words.size() );

            //w[i][j]表示把第[i,j]个单词放入到一行时，该行还剩下的空格的立方。
            //如果一行放不下这么多字符，则w[i][j]为正无穷；当i>j时，w[i][j]为默认值0。
            vector<vector<int>> w( words.size(), vector<int>( words.size() ) );
            for ( size_t i = 0; i < words.size(); ++i )
            {
                for ( size_t j = i; j < words.size(); ++j )
                {
                    int least_length = max_length;
                    for ( size_t k = i; k <= j; ++k )
                    {
                        least_length -= words[k].size();
                    }
                    least_length -= ( j - i );	//还有空格需要注意

                    if ( least_length >= 0 )
                    {
                        w[i][j] = pow( least_length, 1.0 );
                    }
                    else
                    {
                        w[i][j] = numeric_limits<int>::max();
                    }
                }
            }

            for ( int i = 1; i < words.size(); ++i )
            {
                vector<int> vi;
                for ( int k = 0; k < i; ++k )
                {
                    //从words[k]之后进行分行
                    if ( w[k + 1][i] != numeric_limits<int>::max() )
                    {
                        vi.push_back( cost[k] + w[k + 1][i] );
                    }

                }
                auto min_iter = min_element( vi.begin(), vi.end() );
                cost[i] = *min_iter;
            }

            return cost[words.size() - 1];
        }



        /// @brief 正确的整齐打印的动态规划算法
        ///
        /// 动态规划得到的应该是最优解，而不是次优解。\n
        /// 这是本题的正确解法，该动态规划的结果与贪心算法的结果相同，同样也验证了对于本题来说：贪心算法能够给出最优解！\n
        int NeatlyPrintViaDyanmicProgramming( vector<string> const &words, int const max_length )
        {
            size_t size = words.size();

            //一个区间的信息
            struct Message
            {
                int Cost;
                int FirstLineCharNumber;
                int LastLineCharNumber;
                int LineNumber;

                bool operator<( Message const &other )
                {
                    return this->Cost < other.Cost;
                }
            };
            //c[i][j]表示[i,j]个单词之间的整齐打印信息
            vector<vector<Message>> c( size, vector<Message>( size ) );

            //预处理长度为1（即单个点）的情况
            for ( size_t i = 0; i < size; ++i )
            {
                c[i][i].Cost = 0;
                c[i][i].FirstLineCharNumber = words[i].size();
                c[i][i].LastLineCharNumber = words[i].size();
                c[i][i].LineNumber = 1;
            }

            //递推长度>1时的情况
            for ( int length = 2; length <= size; ++length )
            {
                for ( size_t i = 0; i < size - length + 1; ++i )
                {
                    size_t j = i + length - 1;

                    vector<Message> vi;
                    for ( size_t k = i; k < j; ++k )
                    {
                        //将c[i][j]分为c[i][k]、c[k+1][j]两个部分
                        Message a_message;
                        //c[i][k]、c[k+1][j]两部分的首尾行是否能结合为一行
                        bool	can_link_2_parts = c[i][k].LastLineCharNumber + c[k + 1][j].FirstLineCharNumber + 1 <= max_length;

                        //LineNumber
                        a_message.LineNumber = c[i][k].LineNumber + c[k + 1][j].LineNumber;
                        if ( can_link_2_parts )
                        {
                            //首尾两行能结合为一行
                            --a_message.LineNumber;
                        }

                        //FirstLineCharNumber
                        if ( can_link_2_parts && c[i][k].LineNumber == 1 )
                        {
                            //首尾两行能结合为一行 且 前部分只有一行
                            a_message.FirstLineCharNumber = c[i][k].FirstLineCharNumber + c[k + 1][j].FirstLineCharNumber + 1;
                        }
                        else
                        {
                            a_message.FirstLineCharNumber = c[i][k].FirstLineCharNumber;
                        }

                        //LastLineCharNumber
                        if ( can_link_2_parts && c[k + 1][j].LineNumber == 1 )
                        {
                            //首尾两行能结合为一行 且 后部分只有一行
                            a_message.LastLineCharNumber = ( c[i][k].LastLineCharNumber + c[k + 1][j].FirstLineCharNumber + 1 );
                        }
                        else
                        {
                            a_message.LastLineCharNumber = c[k + 1][j].LastLineCharNumber;
                        }

                        //Cost
                        if ( can_link_2_parts )
                        {
                            //首尾两行能结合为一行
                            a_message.Cost = c[i][k].Cost + c[k + 1][j].Cost + ( max_length - c[i][k].LastLineCharNumber - c[k + 1][j].FirstLineCharNumber - 1 );
                        }
                        else
                        {
                            a_message.Cost = c[i][k].Cost + c[k + 1][j].Cost + ( max_length - c[i][k].LastLineCharNumber );
                        }
                        if ( a_message.LineNumber == 1 )
                        {
                            //因为最后一行的空格不算作代价
                            a_message.Cost = 0;
                        }

                        vi.push_back( a_message );
                    }

                    c[i][j] = *min_element( vi.begin(), vi.end() );
                }
            }

            return c[0][words.size() - 1].Cost;
        }
    }

    /// @brief 整齐打印
    ///
    /// 题目描述：考虑在一个打印机上整齐地打印一段文章的问题。输入的正文是n个长度分别l[1], l[2], ..., l[n]（以字符个数度量）的单词构成的序列。
    /// 我们希望将这个段落在一些行上整齐地打印出来，每行至多M个字符。“整齐度”的标准如下。如果某一行包含从i到j的单词，i<j，且单词之间只留一个空，
    /// 则在行末多余的空格字符个数为M-j+i-∑(k=i to j)l[k]，它必须是非负值才能让该行容纳这些单词。我们希望所有行（除了最后一行）的行末多余空格
    /// 字符个数的立方的总和最小。请给出一个动态规划算法，来在打印机上整齐地打印出一段有n个单词的文章。
    void NeatlyPrint()
    {
        //随机生成正文中字符串
        vector<string> words;
        for ( int i = 0; i < 100; ++i )
        {
            //任意字符串长度为1..10
            int length = rand() % 10 + 1;

            string a_word;
            for ( int j = 0; j < length; ++j )
            {
                a_word.push_back( 'a' + rand() % 26 );
            }
            words.push_back( a_word );
        }

        //一行的最大字符数，要保证k_MAX_LENGTH大于任意一个单词的长度，否则无法放下
        int const k_MAX_LEGNTH = 48;

        cout << endl << "贪心算法得出的最小代价（除了最后一行）为：" << NeatlyPrintViaGreedy( words, k_MAX_LEGNTH );
        cout << endl << "动态规划得出的最小代价（除了最后一行）为：" << NeatlyPrintViaDyanmicProgramming( words, k_MAX_LEGNTH );
        cout << endl << "网上流传的错误的动态规划算法得出的最小代价（除了最后一行）为：" << NeatlyPrintViaDyanmicProgrammingWrong( words, k_MAX_LEGNTH );
        cout << endl;
    }

}