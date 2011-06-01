// 15_动态规划.cpp : Defines the entry point for the console application.

//最长公共子串问题：一个给定序列的子序列是在该序列中删去若干元素后得到的序列。给定两个序列X和Y，当另一序列Z既是X的子序列又是Y的子序列时，称Z是序列X和Y的公共子序列。
//最长公共子串就是求给定两个序列的一个最长公共子序列。例如，X=“ABCBDAB”，Y=“BCDB”是X的一个子序列。

//	问题分析：
//	给定两个序列A和B，称序列Z是A和B的公共子序列，是指Z同是A和B的子序列。问题要求已知两序列A和B的最长公共子序列。如采用列举A的所有子序列，并一一检查其是否又是B的子序列，并随时记录所发现的子序列，最终求出最长公共子序列。这种方法因耗时太多而不可取。
//	考虑最长公共子序列问题如何分解成子问题，设A=“a0，a1，…，am-1”，B=“b0，b1，…，bm-1”，并Z=“z0，z1，…，zk-1”为它们的最长公共子序列。不难证明有以下性质： 
//	（1） 如果am-1=bn-1，则zk-1=am-1=bn-1，且“z0，z1，…，zk-2”是“a0，a1，…，am-2”和“b0，b1，…，bn-2”的一个最长公共子序列； 
//	（2） 如果am-1!=bn-1，则若zk-1!=am-1，蕴涵“z0，z1，…，zk-1”是“a0，a1，…，am-2”和“b0，b1，…，bn-1”的一个最长公共子序列； 
//	（3） 如果am-1!=bn-1，则若zk-1!=bn-1，蕴涵“z0，z1，…，zk-1”是“a0，a1，…，am-1”和“b0，b1，…，bn-2”的一个最长公共子序列。 
//	这样，在找A和B的公共子序列时，如有am-1=bn-1，则进一步解决一个子问题，找“a0，a1，…，am-2”和“b0，b1，…，bm-2”的一个 最长公共子序列；如果am-1!=bn-1，则要解决两个子问题，找出“a0，a1，…，am-2”和“b0，b1，…，bn-1”的一个最长公共子序列 和找出“a0，a1，…，am-1”和“b0，b1，…，bn-2”的一个最长公共子序列，再取两者中较长者作为A和B的最长公共子序列。
//	为了节约重复求相同子问题的时间，引入一个数组，不管它们是否对最终解有用，把所有子问题的解存于该数组中，这就是动态规划法所采用的基本方法，具体说明如下。 
//	定义c[i][j]为序列“a0，a1，…，ai-2”和“b0，b1，…，bj-1”的最长公共子序列的长度，计算c[i][j]可递归地表述如下： 
//	（1）c[i][j] = 0                         如果i=0或j=0； 
//	（2）c[i][j] = c[i-1][j-1]+1             如果i,j>0，且a[i-1] == b[j-1]； 
//	（3）c[i][j] = max{c[i][j-1], c[i-1][j]} 如果i,j>0，且a[i-1] != b[j-1]。 
//	按此算式可写出计算两个序列的最长公共子序列的长度函数。由于c[i][j]的产生仅依赖于c[i-1][j-1]、c[i-1][j]和c[i][j-1]，故可以从c[m][n]开始，跟踪c[i][j]的产生过程，逆向构造出最长公共子序列。细节见程序。 

#include "stdafx.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
using namespace std;

namespace chapter15
{

	namespace	//匿名命名空间
	{
		string const LeftHandSide = "abcbdabbadbcddbacbdad";
		string const RightHandSide = "bdcaadcbdbddababba";


		/// 普通的递归,时间复杂度为n的指数级
		int LongestCommonSubsequenceViaRecurise(int i, int j)
		{
			if (i == 0 || j == 0)
			{
				return 0;
			}
			else
			{
				if (LeftHandSide[i - 1] == RightHandSide[j - 1])
				{
					return LongestCommonSubsequenceViaRecurise(i - 1, j - 1) + 1;
				}
				else
				{
					return max(LongestCommonSubsequenceViaRecurise(i, j - 1), LongestCommonSubsequenceViaRecurise(i - 1, j));
				}
			}
		}



		/// 加入了备忘录功能的递归
		/// 
		/// 加入备忘录之后的运行速度超级大幅度的提升,在递归会进行很多重复的子问题的计算的情况下,这样一个小小的改动,极大的提高了效率
		/// 两点改动就可以增加备忘录的功能
		/// 1:在最前面加一个判断,是否已经计算过了,如果已经计算过了就直接取结果返回.
		/// 2:每次要返回之前先把结果保存一下.
		int _LongestCommonSubsequenceViaMemoize(int i, int j, vector< vector<int> > &memoize, vector< vector<int> > &chooses)
		{
			//这个检查备忘录的过程就是“备忘录”方法与普通递归的区别
			//所以说备忘录方法简单好用，只需要使用递归的思维，再加上一个备忘子递归结果的memoize就可以了。
			if (memoize[i][j] != -1)
			{//先检查已经存在于备忘录中
				return memoize[i][j];
			}

			if (i == 0 || j == 0)
			{
				memoize[i][j] = 0;
				return 0;
			}
			else
			{
				if (LeftHandSide[i - 1] == RightHandSide[j - 1])
				{					
					//写备忘录
					memoize[i][j] = _LongestCommonSubsequenceViaMemoize(i - 1, j - 1, memoize, chooses) + 1;
					chooses[i][j] = 0;			//记录选择for step 4
				}
				else
				{
					int i_sub = _LongestCommonSubsequenceViaMemoize(i - 1, j, memoize, chooses);
					int j_sub = _LongestCommonSubsequenceViaMemoize(i, j - 1, memoize, chooses);

					chooses[i][j] = (i_sub > j_sub ? -1 : 1);		//记录选择for step 4
					memoize[i][j] = max(i_sub, j_sub);				//写备忘录
				}

				//备忘录中已经保存当前的子问题的解，直接从备忘录中取出来返回
				return memoize[i][j];
			}
		}

		void LongestCommonSubsequenceViaMemoize()
		{
			//LongestCommonSubsequenceViaMemoize方法的备忘录。
			//memoize[i][j]意味着:取LeftHandSide的前i个字符得到的子字符串与取RightHandSide的前j个字符得到的子字符串 的最长公共子序列的长度
			static vector< vector<int> > memoize;
			//初始化memoize数组,使得其为memoize[0..m, 0..n]
			memoize.resize(LeftHandSide.size() + 1);
			for_each(memoize.begin(), memoize.end(), [](vector<int> &v){v.resize(RightHandSide.size() + 1, -1);});
			//记录递归中每一次所做出的选择，用以在最后构造LCS，这是动态规划中的步骤4
			vector< vector<int> > chooses = memoize;

			//执行递归，求得LCS的长度，同时记录下了每次递归做出的选择
			cout << _LongestCommonSubsequenceViaMemoize(LeftHandSide.size(), RightHandSide.size(), memoize, chooses) << endl;

			//动态规划的step 4：构造出最优解（LCS）
			//无论是使用备忘录的方法还是传统的动态规划方法，最后构造出最优解（LCS）都不太简单
			//总结step4的思路是这样的：
			//在动态规划或者递归的过程中，就需要记录下每次做出的选择，这里用chooses[i][j]保存
			//最后根据每一步所做的选择来反向构造出最优解（LCS）
			string left = LeftHandSide;
			string right = RightHandSide;
			vector<char> result;
			while (!left.empty() && !right.empty())
			{
				int choose = chooses[left.size()][right.size()];
				if (choose == 0)
				{
					result.push_back(left[left.size() - 1]);
					left.erase(left.end() - 1);
					right.erase(right.end() - 1);
				}
				else if (choose == -1)
				{
					left.erase(left.end() - 1);
				}
				else
				{
					right.erase(right.end() - 1);
				}
			}
			reverse(result.begin(), result.end());
			copy(result.begin(), result.end(), ostream_iterator<char>(cout));
			cout << endl;
		}

		/// 正统的动态规划
		/// 
		/// 正统的动态规划与带备忘录功能的递归的区别就在于:一个是自底向上的,一个是从顶向下
		/// 正统的动态规划与带备忘录功能的递归虽然有着相同的渐近复杂度,但是正统的动态规划往往有着更好的常数因子,因而效率更高
		void LongestCommonSubsequenceViaDynamicProgramming()
		{
			//lcs[i][j]意味着:取LeftHandSide的前i个字符得到的子字符串与取RightHandSide的前j个字符得到的子字符串 的最长公共子序列的长度
			static vector< vector<int> > lcs;
			//初始化lcs数组,使得其为lcs[0..m, 0..n]
			lcs.resize(LeftHandSide.size() + 1);
			for_each(lcs.begin(), lcs.end(), [](vector<int> &v){v.resize(RightHandSide.size() + 1, 0);});

			for (size_t i = 1; i <= LeftHandSide.size(); ++i)
			{
				for (size_t j = 1; j <= RightHandSide.size(); ++j)
				{
					if (LeftHandSide[i - 1] == RightHandSide[j - 1])
					{
						lcs[i][j] = lcs[i - 1][j - 1] + 1;
					}
					else
					{
						lcs[i][j] = max(lcs[i][j - 1], lcs[i - 1][j]);
					}
				}
			}

			cout << lcs[LeftHandSide.size()][RightHandSide.size()] << endl;
		}

	}

	int LongestCommonSubsequence()
	{
		LongestCommonSubsequenceViaMemoize();

		cout << LongestCommonSubsequenceViaRecurise(LeftHandSide.size(), RightHandSide.size()) << endl;		
		LongestCommonSubsequenceViaDynamicProgramming();

		return 0;
	}

}