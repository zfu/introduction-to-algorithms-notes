// Intorduction to Algorithms.cpp : 定义控制台应用程序的入口点。
//



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
{
    //动态规划
    void AssembleDispatch();				//装配线调度
    void MatrixListMultiply();				//矩阵链乘法
    int LongestCommonSubsequence();			//最长公共子序列
    void BestBinarySearchTree();			//最优二叉查找树
    void NeatlyPrint();						//整齐打印
    void PlanParty();						//聚会计划
    void BitonicTour();						//双调旅程

}
namespace chapter16
{
	//贪心算法
    void HuffmanCode();						//哈夫曼编码
}
namespace chapter18
{
	//B树
	void test();
}
namespace chapter21
{
	//用于不相交集合的数据结构（并查集）
	void test();
}
namespace chapter22
{
	//图的基本算法 
	void BFS_And_DFS();						//广度&深度优先遍历
	void TopologicalSort();					//拓扑排序
	void StronglyConnectedComponent();		//强连通分支
}
namespace chapter23
{
	//最生成树
	void Kruskal();
	void Prim();
}
namespace chapter24
{
	//单源最短路径
	bool testBellmanFord();
	void testDijkstra();
}
namespace chapter25
{
	//每对顶点间的最短路径
	void FloydWarshall();
	void Johnson();
}
namespace chapter26
{
	//最大流
	void testFordFulkerson();
}

int _tmain( int argc, _TCHAR* argv[] )
{
    chapter26::testFordFulkerson();
    return 0;
}

