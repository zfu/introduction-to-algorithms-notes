// Intorduction to Algorithms.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>

namespace chapter6
{
int test();
}
namespace chapter7
{
int test();
}
namespace chapter8
{
int test();
}
namespace chapter9
{
int test();
}
namespace chapter11
{
int test();
}
namespace chapter12
{
int test();
}
namespace chapter13
{
int test();
}
namespace chapter15
{//动态规划

void AssembleDispatch();				//装配线调度
void MatrixListMultiply();				//矩阵链乘法
int LongestCommonSubsequence();			//最长公共子序列
void BestBinarySearchTree();			//最优二叉查找树
void NeatlyPrint();						//整齐打印
void PlanParty();						//聚会计划
void BitonicTour();						//双调旅程

}


int _tmain( int argc, _TCHAR* argv[] )
{
    //chapter13::test();
    chapter15::BitonicTour();
	int a[2][2] ={{1,2},{2,3}};
    return 0;
}

