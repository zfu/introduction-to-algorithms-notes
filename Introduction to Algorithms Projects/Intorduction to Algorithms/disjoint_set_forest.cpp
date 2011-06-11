#include "stdafx.h"
#include "disjoint_set_forest.h"
using namespace std;

namespace chapter21
{
	void test()
	{
		vector<DisjointSetForest::DisjointSet<int> *> nodes;
		for (int i = 0; i < 10; ++i)
		{
			nodes.push_back(DisjointSetForest::MakeSet(i));
		}


		for (int i = 0; i < 5; ++i)
		{
			int index1 = rand() % 10;
			int index2 = rand() % 10;
			DisjointSetForest::Union(nodes[index1], nodes[index2]);
			cout << "Union : " << index1 << "---" << index2 << endl;
		}

		for (int i = 0; i < 10; ++i)
		{
			cout << i << "-------------->" << DisjointSetForest::FindSet(nodes[i])->Item << endl;
		}

		//for (int i = 0; i < nodes.size(); ++i)
		//{
		//	FindSet(nodes[i]);
		//	cout << FindSet(nodes[i])->Item << endl;
		//}
	}
}