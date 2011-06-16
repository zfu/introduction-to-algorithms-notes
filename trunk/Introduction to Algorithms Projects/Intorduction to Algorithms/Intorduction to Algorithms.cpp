// Intorduction to Algorithms.cpp : 定义控制台应用程序的入口点。
//



#include <iostream>

namespace ita
{
	//第6章：堆排序
    int testHeapSort();

	//第7章：快速排序
    int testQuickSort();

	//第8章：线性时间排序
    int testLinearSort();

	//第9章：中位数和顺序统计学
    int testNthElement();

	//第11章：散列表
    int testHashTable();

	//第12章：二叉查找树
    int testBinarySearchTree();

	//第13章：红黑树
    int testRedBlackTree();

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
	void testBTree();

	//第19章：二项堆
	void testBinomialHeaps();

	//第20章：斐波那契堆
	int testFibonacciHeap();				 

	//第21章：用于不相交集合的数据结构（并查集）
	void testDisjointSetForest();

	//第22章：图的基本算法 
	void BFS_And_DFS();						//广度&深度优先遍历
	void TopologicalSort();					//拓扑排序
	void StronglyConnectedComponent();		//强连通分支

	//第23章：最小生成树
	void Kruskal();
	void Prim();

	//第24章：单源最短路径
	bool testBellmanFord();
	void testDijkstra();

	//第25章：每对顶点间的最短路径
	void FloydWarshall();
	void Johnson();

	//第26章：最大流
	void testFordFulkerson();
}

int _tmain( int argc, _TCHAR* argv[] )
{
    ita::testHeapSort();
    return 0;
}

