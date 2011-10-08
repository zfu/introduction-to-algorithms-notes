//////////////////////////////////////////////////////////////////////////
/// @file		intorduction_to_algorithms.cpp
/// @brief		主调函数Main
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
/// 2011/06/17	1.0	谭川奇	创建


#include <iostream>

namespace ita
{
    //第6章：堆排序
    int testHeapSort();						//堆排序

    //第7章：快速排序
    int testQuickSort();					//快速排序

    //第8章：线性时间排序
    int testLinearSort();					//线性时间排序

    //第9章：中位数和顺序统计学
    int testNthElement();					//中位数和顺序统计学

    //第11章：散列表
    int testHashTable();					//散列表

    //第12章：二叉查找树
    int testBinarySearchTree();				//二叉查找树

    //第13章：红黑树
    int testRedBlackTree();					//红黑树

    //第15章：动态规划
    void AssembleDispatch();				//装配线调度
    void MatrixListMultiply();				//矩阵链乘法
    int LongestCommonSubsequence();			//最长公共子序列
    void BestBinarySearchTree();			//最优二叉查找树
    void NeatlyPrint();						//整齐打印
    void PlanParty();						//聚会计划
    void BitonicTour();						//双调旅程

    //第16章：贪心算法
    void HuffmanCode();						//哈夫曼编码

    //第18章：B树
    void testBTree();						//B树

    //第19章：二项堆
    void testBinomialHeaps();				//二项堆

    //第20章：斐波那契堆
    int testFibonacciHeap();				//斐波那契堆

    //第21章：用于不相交集合的数据结构
    void testDisjointSetForest();			//用于不相交集合的数据结构（并查集）

    //第22章：图的基本算法
    void testBreadthFirstSearch();			//广度优先遍历
    void testDeapthFirstSearch();			//深度优先遍历
    void TopologicalSort();					//拓扑排序
    void StronglyConnectedComponent();		//强连通分支

    //第23章：最小生成树
    void Kruskal();							//Kruskal最小生成树算法
    void Prim();							//Prim最小生成树算法

    //第24章：单源最短路径
    bool testBellmanFord();					//BellmanFord单源最短路径算法
    void testDijkstra();					//Dijkstra单源最短路径算法

    //第25章：每对顶点间的最短路径
    void FloydWarshall();					//FloydWarshall每对顶点最短路径算法
    void Johnson();							//Johnson每对顶点最短路径算法

    //第26章：最大流
    void testFordFulkerson();				//FordFulkerson最大流算法
}

using namespace ita;
int _tmain( int argc, _TCHAR* argv[] )
{
    //testHeapSort();

    //testQuickSort();

    //testLinearSort();

    //testNthElement();

    //testHashTable();

    //testBinarySearchTree();

    //testRedBlackTree();

    //AssembleDispatch();
    //MatrixListMultiply();
    //LongestCommonSubsequence();
    //BestBinarySearchTree();
    //NeatlyPrint();
    //PlanParty();
    //BitonicTour();

    //HuffmanCode();

    //testBTree();

    //testBinomialHeaps();

    //testFibonacciHeap();

    //testDisjointSetForest();

    //testBreadthFirstSearch();
    //testDeapthFirstSearch();
    //TopologicalSort();
    //StronglyConnectedComponent();

    //Kruskal();
    //Prim();

    //testBellmanFord();
    //testDijkstra();

    //FloydWarshall();
    //Johnson();

    testFordFulkerson();
    return 0;
}

