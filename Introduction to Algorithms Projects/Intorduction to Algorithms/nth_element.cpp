//////////////////////////////////////////////////////////////////////////  
///    COPYRIGHT NOTICE  
///    Copyright (c) 2009
///    All rights reserved.  
///  
/// @file		nth_element.cpp
/// @brief		nth_element.cpp的简短描述 
///  			
///  			nth_element.cpp的详细描述
/// 
/// @author		谭川奇	chuanqi.tan@gmail.com 
/// @date		2011/05/26
/// @version 	1.0
///  
///  
///    修订说明：最初版本  
//////////////////////////////////////////////////////////////////////////  


#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
using namespace std;

namespace chapter9
{
	/// 寻找v数组的子集的第i个顺序统计量
	/// 
	/// 寻找v数组的子集[begin_index, end_index]中的第i个元素顺序统计量，0<=i<size
	int _NthElement(vector<int> &v, int const begin_index, int const end_index, int const n)
	{
		//这个判断纯粹只是一个加速return的技巧，没有这个判断算法也是正确的！
		if (begin_index == end_index)
		{
			return v[begin_index];
		}

		//随机取样
		int swap_index = rand() % (end_index - begin_index + 1) + begin_index;
		swap(v[swap_index], v[end_index]);

		//根据最后一个主元进行分割成两部分
		int i = begin_index;
		for (int j = begin_index; j < end_index; ++j)
		{
			if (v[j] < v[end_index])
			{
				swap(v[i++], v[j]);
			}
		}
		swap(v[i], v[end_index]);

		//主元是本区间的第k个元素顺序统计量，0<=k<size
		int k = i - begin_index;

		if (n == k)
		{//找到了
			return v[i];
		}
		if (n < k)
		{//在左区间继续找
			return _NthElement(v, begin_index, i - 1, n);
		}
		else
		{//在右区间继续找：由于主元是第k个元素顺序统计量(0<=k<size)，所以小于等于主元的元素有k+1个（包括主元），因此寻找右区间的第n-(k+1)个顺序统计量
			return _NthElement(v, i + 1, end_index, n - k - 1);
		}
	}

	/// 寻找v数组中的第i个顺序统计量，0<=i<size
	int NthElement(vector<int> &v, int const n)
	{
		return _NthElement(v, 0, v.size() - 1, n);
	}

	int test()
	{
		vector<int> v;
		for (int i = 0; i < 10; ++i)
		{
			v.push_back((rand() % 1000));
		}
		copy(v.begin(), v.end(), ostream_iterator<int>(cout, "  "));
		cout << endl;

		for (int i = 0; i < 10; ++i)
		{
			cout << i << "th element is:" << NthElement(v, i) << endl;
		}

		return 0;
	}
}