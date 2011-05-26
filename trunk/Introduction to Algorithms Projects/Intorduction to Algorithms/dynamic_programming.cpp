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

using namespace std;

namespace chapter15
{

string LeftHandSide = "abcbdabbadbcddbacbdad";
string RightHandSide = "bdcaadcbdbddababba";

//lcs[i][j]意味着:
//取LeftHandSide的前i个字符得到的子字符串与取RightHandSide的前j个字符得到的子字符串 的最长公共子序列的长度
vector< vector<int> > lcs;


//普通的递归,时间复杂度为n的指数级
int Calc1(int i, int j)
{
	if (i == 0 || j == 0)
	{
		return 0;
	}
	else
	{
		if (LeftHandSide[i - 1] == RightHandSide[j - 1])
		{
			return Calc1(i - 1, j - 1) + 1;
		}
		else
		{
			return max(Calc1(i, j - 1), Calc1(i - 1, j));
		}
	}
}

//加入了备忘录功能的递归
//加入备忘录之后的运行速度超级大幅度的提升,在递归会进行很多重复的子问题的计算的情况下,这样一个小小的改动,极大的提高了效率
//两点改动就可以增加备忘录的功能
//1:在最前面加一个判断,是否已经计算过了,如果已经计算过了就直接取结果返回.
//2:每次要返回之前先把结果保存一下.
int Calc2(int i, int j)
{
	if (lcs[i][j] != -1)
	{
		return lcs[i][j];
	}

	if (i == 0 || j == 0)
	{
		lcs[i][j] = 0;
		return 0;
	}
	else
	{
		if (LeftHandSide[i - 1] == RightHandSide[j - 1])
		{
			int temp = Calc2(i - 1, j - 1) + 1;
			lcs[i][j] = temp;
			return temp;
		}
		else
		{
			int temp = max(Calc2(i, j - 1), Calc2(i - 1, j));
			lcs[i][j] = temp;
			return temp;
		}
	}
}

//正统的动态规划
//正统的动态规划与带备忘录功能的递归的区别就在于:一个是自底向上的,一个是从顶向下
//正统的动态规划与带备忘录功能的递归虽然有着相同的渐近复杂度,但是正统的动态规划往往有着更好的常数因子,因而效率更高
void Calc3()
{
	for (int i = 0; i <= LeftHandSide.size(); ++i)
	{
		lcs[i][0] = 0;
	}
	for (int i = 0; i <= RightHandSide.size(); ++i)
	{
		lcs[0][i] = 0;
	}

	for (int i = 1; i <= LeftHandSide.size(); ++i)
	{
		for (int j = 1; j <= RightHandSide.size(); ++j)
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

int test()
{
	//初始化lcs数组,使得其为lcs[0..m, 0..n]
	lcs.clear();
	for (size_t i = 0; i < LeftHandSide.size() + 1; ++i)
	{
		vector<int> tmp(RightHandSide.size() + 1, -1);
		lcs.push_back(tmp);
	}


	cout << Calc1(LeftHandSide.size(), RightHandSide.size()) << endl;
	cout << Calc2(LeftHandSide.size(), RightHandSide.size()) << endl;
	Calc3();

	return 0;
}

}