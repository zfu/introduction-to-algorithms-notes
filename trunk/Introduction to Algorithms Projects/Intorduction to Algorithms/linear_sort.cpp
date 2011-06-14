//////////////////////////////////////////////////////////////////////////  
///    COPYRIGHT NOTICE  
///    Copyright (c) 2009
///    All rights reserved.  
///  
/// @file		liner_sort.cpp
/// @brief		liner_sort.cpp的简短描述 
///  			
///  			liner_sort.cpp的详细描述
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

namespace chapter8
{
	/// 计数排序
	void counting_sort()
	{
		int const k_max_size = 100;		//待排序的所有元素都必须位于区间[0, k_max_size)
		//初始化区间为[0, k_max_size)之间的随机数作为输入
		std::vector<int> v;
		for (int i = 0; i < 100; ++i)
		{
			v.push_back(rand() % k_max_size);
		}

		//进行计数 c[i] = j代表着i在输入数据中出现了j次
		std::vector<int> c(k_max_size, 0);
		std::for_each(v.begin(), v.end(), [&](int i){++c[i];});

		//对所有的计数从依次总结出最后的排序，并没有使用原书上的方法
		//比书上的算法更直接，将二步合成了一步，效率上是同样的渐近时间复杂度的，似乎更好点。
		v.clear();
		for (int i = 0; i < k_max_size; ++i)
		{
			for (int k = 0; k < c[i]; ++k)
			{
				v.push_back(i);
			}
		}

		copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, "  "));
	}

	/// 基数排序时值最大的维数：3意味着元素的区间为[0, 999]
	int const k_max_dim = 3;

	/// 基数排序
	void radix_sort(std::vector<int> &v, int dim)
	{
		//根据第dim维的大小进行“稳定”的排序，这个stable很重要，是基数排序的基石
		stable_sort(v.begin(), v.end(), [&](int left, int right) -> bool{
			//得到一个数的某一维的值
			//eg:	GetDim(987, 1) = 9
			//		GetDim(987, 2) = 8
			//		GetDim(987, 3) = 7
			auto GetDim = [](int number, int d) -> int
			{
				for (int i = 1; i <= k_max_dim - d; ++i)
				{
					number /= 10;
				}
				return number % 10;
			};
			return GetDim(left, dim) < GetDim(right, dim);
		});

		if (dim > 1)
		{
			radix_sort(v, dim - 1);
		}
	}

	/// 基础排序算法的初始化和调用
	void radix_sort_caller()
	{
		//初始化[0,999]之间的随机数作为输入
		std::vector<int> v;
		for (int i = 0; i < 10; ++i)
		{
			v.push_back(rand() % static_cast<int>(pow(10.0, static_cast<double>(k_max_dim))));
		}
		std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, "  ")); std::cout << std::endl;

		radix_sort(v, k_max_dim);
		std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, "  "));
	}

	/// 桶排序
	void bucket_sort()
	{
		//初始化[0,1)之间的随机数
		vector<double> v;
		for (int i = 0; i < 10; ++i)
		{
			v.push_back((rand() % 100) * 1.0 / 100.0);
		}
		copy(v.begin(), v.end(), ostream_iterator<double>(cout, "  "));
		cout << endl;

		//构建桶，并将所有的元素放入到相应的桶中去
		vector< vector<double> > bucket(10);	//10个桶
		for_each(v.begin(), v.end(), [&](double d){
			bucket[d * 10].push_back(d);
		});

		//对每一个桶里的元素进行排序
		for_each(bucket.begin(), bucket.end(), [](vector<double> &sub_v){
			sort(sub_v.begin(), sub_v.end());
		});

		//依次把每个桶中的元素提取出来并组合在一起
		v.clear();
		for_each(bucket.begin(), bucket.end(), [&](vector<double> &sub_v){
			v.insert(v.end(), sub_v.begin(), sub_v.end());
		});

		copy(v.begin(), v.end(), ostream_iterator<double>(cout, "  "));
	}

	int test()
	{
		cout << endl << "===========开始计数排序===========" << endl;
		counting_sort();
		cout << endl << "===========开始基数排序===========" << endl;
		radix_sort_caller();
		cout << endl << "===========开始桶排序===========" << endl;
		bucket_sort();

		getchar();
		return 0;
	}
}