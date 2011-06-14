//////////////////////////////////////////////////////////////////////////  
///    COPYRIGHT NOTICE  
///    Copyright (c) 2009
///    All rights reserved.  
///  
/// @file		matrix_list_multiply.cpp
/// @brief		matrix_list_multiply.cpp的简短描述 
///  			
///  			matrix_list_multiply.cpp的详细描述
/// 
/// @author		谭川奇	chuanqi.tan(at)gmail.com 
/// @date		2011/05/31
/// @version 	1.0
///  
///  
///    修订说明：最初版本  
//////////////////////////////////////////////////////////////////////////  



#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <iomanip>
#include <limits>
#include <fstream>
using namespace std;

namespace chapter15
{
	static void Print(vector< vector<int> > const &pos, int i, int j)
	{
		if (i  == j)
		{
			cout << "A" << i;
		}
		else
		{
			cout << "(";
			Print(pos, i, pos[i][j]);
			Print(pos, pos[i][j] + 1, j);
			cout << ")";
		}
	}

	/// 矩阵链乘法问题
	void MatrixListMultiply()
	{
		int maxtix_length[] = {30, 35, 15, 5, 10, 20, 25};

		//初始化一个6*6的二维vector
		//times[i][j]表示从第i个矩阵到第j个矩阵之间相乘的最少计算次数，当i>j时没有意义
		vector< vector<int> > times;
		times.resize(6);
		for_each(times.begin(), times.end(), [](vector<int> &v){v.resize(6);});
		//构造一个6*6的二维数组来保存每次对序列进行最优划分的位置
		//k_pos[i][j]表示在计算从第i个矩阵到第j个矩阵之间相乘的最少计算次数时，应该从k_pos[i][j]进行划分
		auto k_pos = times;

		for (int l = 2; l <= 6; ++l)
		{//计算长度为l的序列所需要的最少计算次数。按长度递增，因为短的序列必须在长的序列之前进行计算
			for (int i = 0; i < 6 - l + 1; ++i)
			{
				int j = i + l - 1;
				times[i][j] = numeric_limits<int>::max();

				for (int k = i; k < j; ++k)
				{
					int value = times[i][k] + times[k + 1][j] + maxtix_length[i] * maxtix_length[k + 1] * maxtix_length[j + 1];
					if (value < times[i][j])
					{
						times[i][j] = value;
						k_pos[i][j] = k;
					}					
				}
			}
		}

		//for (int i = 0; i < 6; ++i)
		//{
		//	for (int j = 0; j < 6; ++j)
		//	{
		//		cout << times[i][j] << "\t";
		//	}
		//	cout << endl;
		//}
		cout << times[0][5] << endl;
		Print(k_pos, 0, 5);

		getchar();
	}
}